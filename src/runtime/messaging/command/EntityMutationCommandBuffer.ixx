/**
 * @file EntityCommandBuffer.ixx
 * @brief Deferred command buffer for structural entity mutations (add/remove components, activate/deactivate).
 */
module;

#include <memory>
#include <vector>
#include <cassert>


#include "helios-engine-config.h"

export module helios.engine.runtime.messaging.command.EntityMutationCommandBuffer;

import helios.engine.runtime.world.ManagerRegistry;
import helios.engine.runtime.world.EntityMutationManager;
import helios.engine.runtime.messaging.command.CommandBuffer;
import helios.engine.runtime.messaging.command.types.CommandBufferTypeId;
import helios.engine.runtime.messaging.command.CommandBufferRegistry;

import helios.engine.runtime.messaging.command.CommandHandlerRegistry;
import helios.engine.runtime.timing.TimerManager;

import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.messaging.command.tags;
import helios.engine.runtime.messaging.command.TypedCommandBuffer;
import helios.ecs.commands;

using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::timing;
using namespace helios::engine::runtime::messaging::command::types;
using namespace helios::engine::runtime::messaging::command::tags;
export namespace helios::engine::runtime::messaging::command {

    /**
     * @brief Collects deferred entity-mutation commands and dispatches them in bulk.
     *
     * Commands are enqueued during system execution and forwarded to the
     * `EntityMutationManager` during the flush phase, decoupling structural
     * ECS mutations from hot iteration loops.
     *
     * Each concrete command type gets its own `InternalBuffer`, created lazily
     * on first use and stored in an internal `CommandBufferRegistry`.
     *
     * @tparam THandle Entity handle type that identifies the target registry.
     */
    template<typename THandle>
    class EntityMutationCommandBuffer {

        CommandBufferRegistry commandBufferRegistry_{};

        CommandHandlerRegistry* commandHandlerRegistry_{nullptr};

        ManagerRegistry* managerRegistry_{nullptr};

        /**
         * @brief Per-command-type storage and dispatch unit.
         *
         * Created lazily by `EntityMutationCommandBuffer::modelFor<TCommand>()`.
         * On `flush()` every buffered command is forwarded to the
         * `CommandHandlerRegistry`, which routes it to the `EntityMutationManager`.
         *
         * @tparam TCommandType ECS command struct to buffer (e.g. `AddComponentCommand<C>`).
         */
        template<typename TCommandType>
        class InternalBuffer {

            CommandHandlerRegistry* commandHandlerRegistry_{nullptr};

            EntityMutationManager<THandle>* entityMutationManager_{nullptr};

            std::vector<TCommandType> commands_;

            public:
            /** @brief Role tag marking this as a command buffer for the engine registry. */
            using EngineRoleTag = CommandBufferRole;

            /** @brief Reserves default capacity for the command vector. */
            InternalBuffer() {
                commands_.reserve(DEFAULT_ENTITY_MUTATION_COMMAND_BUFFER_CAPACITY);
            }

            /**
             * @brief Forwards all buffered commands to the handler registry and clears the buffer.
             *
             * @param updateContext Frame-local update context (passed through to handlers).
             */
            void flush(UpdateContext& updateContext) {

                for (auto& cmd : commands_) {
                    commandHandlerRegistry_->submit<TCommandType>(std::move(cmd));
                }

                clear();
            }

            /** @brief Discards all buffered commands without dispatching them. */
            void clear() {
                commands_.clear();
            }

            /**
             * @brief Wires this buffer to the handler and manager registries.
             *
             * Registers a handler for `TCommandType` on the `EntityMutationManager`
             * bound to `THandle`. Must be called before the first `add()` or `flush()`.
             *
             * @param commandHandlerRegistry Registry used to route commands to their handlers.
             * @param managerRegistry        Registry providing the `EntityMutationManager<THandle>`.
             */
            void init(CommandHandlerRegistry& commandHandlerRegistry, ManagerRegistry& managerRegistry) {
                commandHandlerRegistry_ = &commandHandlerRegistry;

                entityMutationManager_ = managerRegistry.item<EntityMutationManager<THandle>>();

                assert(entityMutationManager_ && "EntityMutationManager for the specified handle is not initialized.");

                commandHandlerRegistry.handleCommands<TCommandType>(*entityMutationManager_);

            }

            /**
             * @brief Enqueues a new command, constructing it in-place from `args`.
             *
             * @tparam TArgs Constructor argument types for `TCommandType`.
             * @param  args  Arguments forwarded to the `TCommandType` constructor.
             */
            template<typename ... TArgs>
            void add(TArgs&&... args) {
                commands_.emplace_back(std::forward<TArgs>(args)...);
            }

        };

        /**
         * @brief Returns (or lazily creates) the `InternalBuffer` for `TCommand`.
         *
         * @tparam TCommand Command type whose buffer is requested.
         * @return Non-owning pointer to the buffer.
         */
        template<typename TCommand>
        auto* modelFor() {

            auto* model = commandBufferRegistry_.item<InternalBuffer<TCommand>>();

            if (!model) {
                auto& created   =
                    commandBufferRegistry_.add<InternalBuffer<TCommand>>(
                        CommandBuffer(InternalBuffer<TCommand>{})
                    );

                assert(commandHandlerRegistry_
                    && managerRegistry_
                    && "CommandHandlerRegistry and ManagerRegistry must be initialized before adding commands.");
                created.init(*commandHandlerRegistry_,*managerRegistry_);



                return &created;
            }

            return model;
        }

    public:

        /** @brief The entity handle type this buffer operates on. */
        using Handle_type = THandle;

        /** @brief Role tag marking this as a command buffer for the engine registry. */
        using EngineRoleTag = CommandBufferRole;

        /**
         * @brief Enqueues a command of type `TCommand`, constructing it from `args`.
         *
         * `TCommand::Handle_type` must match `THandle`.
         *
         * @tparam TCommand ECS command type to enqueue.
         * @tparam Args     Constructor argument types.
         * @param  args     Arguments forwarded to the command constructor.
         */
        template<typename TCommand, typename... Args>
        requires std::is_same_v<typename TCommand::Handle_type, THandle>
        void add(Args&&... args) {

            auto* model = modelFor<TCommand>();

            model->add(std::forward<Args>(args)...);
        }

        /**
         * @brief Returns a raw pointer to the `InternalBuffer` for `TCommand`.
         *
         * Useful when a caller needs direct access to the buffer (e.g. for batched inserts).
         *
         * @tparam TCommand Command type whose buffer is requested.
         * @return Non-owning pointer to the internal buffer.
         */
        template<typename TCommand>
        auto* bufferForCommand() {
            return modelFor<TCommand>();
        }

        /**
         * @brief Initialises this buffer and all already-registered internal buffers.
         *
         * Must be called once before the first `add()` or `flush()`.
         *
         * @param commandHandlerRegistry Registry used to route commands to handlers.
         * @param managerRegistry        Registry providing the `EntityMutationManager<THandle>`.
         */
        void init(CommandHandlerRegistry& commandHandlerRegistry, ManagerRegistry& managerRegistry) {

            commandHandlerRegistry_ = &commandHandlerRegistry;
            managerRegistry_ = &managerRegistry;

            for (auto* buffer : commandBufferRegistry_.items()) {
                buffer->init(commandHandlerRegistry, managerRegistry);
            }
        }

        /**
         * @brief Flushes all internal buffers, dispatching every queued command.
         *
         * @param updateContext Frame-local update context forwarded to each buffer's flush.
         */
        void flush(UpdateContext& updateContext) {
            for (auto* buffer : commandBufferRegistry_.items()) {
                buffer->flush(updateContext);
            }
        }

        /**
         * @brief Discards all queued commands across every internal buffer without dispatching.
         */
        void clear() {
            for (auto* buffer : commandBufferRegistry_.items()) {
                buffer->clear();
            }
        }

    };

}
