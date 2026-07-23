/**
 * @file EntityMutationManager.ixx
 * @brief Applies deferred entity-mutation commands to the ECS world.
 */
module;

#include <vector>
#include <format>
#include "helios-engine-config.h"
#include <optional>
#include <algorithm>
#include <iterator>
#include <cassert>

export module helios.engine.runtime.world.EntityMutationManager;

import helios.ecs.commands;

import helios.engine.runtime.world.tags.ManagerRole;
import helios.engine.runtime.messaging.command.CommandBuffer;
import helios.engine.runtime.messaging.command.CommandBufferRegistry;
import helios.engine.runtime.messaging.command.tags.CommandBufferRole;

import helios.engine.runtime.messaging.command.CommandHandlerRegistry;
import helios.engine.runtime.world.ManagerRegistry;

import helios.ecs.types;

import helios.engine.util.log;
import helios.engine.runtime.world.UpdateContext;

import helios.ecs.concepts.Traits;

using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::world::tags;
using namespace helios::engine::runtime::messaging::command;
using namespace helios::ecs::types;
using namespace helios::ecs::concepts::traits;
using namespace helios::ecs::commands;
using namespace helios::engine::util::log;
using namespace helios::engine::runtime::messaging::command::tags;

#define HELIOS_LOG_SCOPE "helios::engine::runtime::world::EntityMutationManager"
export namespace helios::engine::runtime::world {


    /**
     * @brief Receives submitted ECS mutation commands and applies them during flush.
     *
     * Acts as the write-back stage for `EntityMutationCommandBuffer`: commands
     * queued there are routed here via the `CommandHandlerRegistry` and buffered
     * per command type. On `flush()` each command is resolved against the live
     * `UpdateContext` and executed (add/remove component, activate/deactivate entity).
     *
     * @tparam THandle Entity handle type identifying the target ECS registry.
     */
    template<typename THandle>
    class EntityMutationManager {

        CommandBufferRegistry commandBufferRegistry_{};

        CommandHandlerRegistry* commandHandlerRegistry_{nullptr};


        /**
         * @brief Per-command-type buffer that applies mutations on flush.
         *
         * Two `flush()` overloads exist, selected by concept constraints:
         * one for component add/remove, one for entity activation/deactivation.
         *
         * @tparam TCommandType ECS command type stored in this buffer.
         */
        template<typename TCommandType>
        class InternalBuffer {

            CommandHandlerRegistry* commandHandlerRegistry_{nullptr};

            std::vector<TCommandType> commands_;

            public:
            /** @brief Role tag identifying this as a command buffer in the engine registry. */
            using EngineRoleTag = CommandBufferRole;

            /** @brief Reserves default capacity for the command vector. */
            InternalBuffer() {
                commands_.reserve(DEFAULT_ENTITY_MUTATION_COMMAND_BUFFER_CAPACITY);
            }

            /**
             * @brief Applies all buffered add/remove-component commands and clears the buffer.
             *
             * For each command the target entity is resolved via `updateContext.find()`.
             * Missing entities are silently skipped.
             *
             * @param updateContext Frame-local ECS context used to resolve entity handles.
             */
            void flush(UpdateContext& updateContext)
            requires IsAddComponentCommand_v<TCommandType> || IsRemoveComponentCommand_v<TCommandType> {

                using Component_type = typename TCommandType::Component_type;

                for (auto& command : commands_) {
                    auto entity = updateContext.find<THandle>(command.handle);
                    if (!entity) {
                        continue;
                    }
                    if constexpr (IsAddComponentCommand_v<TCommandType>) {
                        entity->template add<Component_type>(std::move(command.component));
                    } else {
                        entity->template remove<Component_type>();
                    }
                }

                clear();
            }

            /**
             * @brief Applies all buffered activate/deactivate-entity commands and clears the buffer.
             *
             * For each command the target entity is resolved via `updateContext.find()`.
             * Missing entities are silently skipped.
             *
             * @param updateContext Frame-local ECS context used to resolve entity handles.
             */
            void flush(UpdateContext& updateContext)
            requires IsActivateEntityCommand_v<TCommandType> || IsDeactivateEntityCommand_v<TCommandType> {

                for (auto& command : commands_) {
                    auto entity = updateContext.find<THandle>(command.handle);
                    if (!entity) {
                        continue;
                    }
                    if constexpr (IsActivateEntityCommand_v<TCommandType>) {
                        entity->setActive(true);
                    } else {
                        entity->setActive(false);
                    }
                }

                clear();
            }

            /** @brief Discards all buffered commands without applying them. */
            void clear() {
                commands_.clear();
            }

            /** @brief No-op; satisfies the buffer initialisation interface. */
            void init(CommandHandlerRegistry& commandHandlerRegistry, ManagerRegistry& managerRegistry) {/* noop*/ }

            /**
             * @brief Stores a command for deferred execution.
             *
             * @param commandType Command instance to buffer.
             */
            void add(TCommandType commandType) {
                commands_.emplace_back(std::forward<TCommandType>(commandType));
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

                return &created;
            }

            return model;
        }

    public:

        /** @brief The entity handle type this manager operates on. */
        using Handle_type = THandle;

        /** @brief Role tag identifying this as a manager in the engine registry. */
        using EngineRoleTag = ManagerRole;

        /**
         * @brief Accepts a command from the `CommandHandlerRegistry` and enqueues it.
         *
         * `TCommand::Handle_type` must match `THandle`.
         *
         * @tparam TCommand Deduced ECS command type.
         * @param  command  Command instance forwarded into the internal buffer.
         * @return `true` unconditionally (reserved for future error reporting).
         */
        template<typename TCommand>
        requires std::is_same_v<typename TCommand::Handle_type, THandle>
        bool submit(TCommand&& command) {

            using Command_type = std::remove_cvref_t<TCommand>;

            auto* model = modelFor<Command_type>();

            model->add(std::forward<TCommand>(command));

            return true;
        }

        /**
         * @brief Initialises the manager. Currently a no-op.
         *
         * @param commandHandlerRegistry Unused; kept for interface uniformity.
         */
        void init(CommandHandlerRegistry& commandHandlerRegistry) {}

        /**
         * @brief Flushes all internal buffers, applying every queued mutation to the world.
         *
         * @param updateContext Frame-local ECS context forwarded to each buffer's flush.
         */
        void flush(UpdateContext& updateContext) {
            for (auto* buffer : commandBufferRegistry_.items()) {
                buffer->flush(updateContext);
            }
        }


    };


}