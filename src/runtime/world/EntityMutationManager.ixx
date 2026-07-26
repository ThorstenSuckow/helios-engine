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
import helios.ecs.types.ComponentTypeId;

import helios.engine.core.thread.JobSystem;

import helios.engine.runtime.world.tags.ManagerRole;
import helios.engine.runtime.messaging.command.CommandBuffer;
import helios.engine.runtime.messaging.command.types.CommandBufferTypeId;
import helios.engine.runtime.messaging.command.CommandBufferRegistry;
import helios.engine.runtime.messaging.command.tags.CommandBufferRole;

import helios.engine.runtime.messaging.command.CommandHandlerRegistry;
import helios.engine.runtime.world.ManagerRegistry;

import helios.ecs.types;
import helios.ecs.components;


import helios.engine.util.log;
import helios.engine.runtime.world.UpdateContext;

import helios.ecs.concepts.Traits;
import helios.engine.util.log;

using namespace helios::ecs::components;
using namespace helios::engine::core::thread;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::world::tags;
using namespace helios::engine::runtime::messaging::command;
using namespace helios::engine::runtime::messaging::command::types;
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
     * are submitted directly via `submit()` or `submitBatch()` and buffered per
     * command type. On `flush()` / `flushParallel()` each buffer applies its
     * mutations to the entity manager.
     *
     * @tparam TEntityManager Entity manager type identifying the target ECS registry.
     */
    template<typename TEntityManager>
    class EntityMutationManager {

        /** @brief Shorthand for the entity handle type derived from `TEntityManager`. */
        using THandle = typename TEntityManager::Handle_type;

        /** @brief Reference to the entity manager mutations are applied to. */
        TEntityManager& entityManager_;

        /** @brief Registry of lazily created per-command-type `InternalBuffer` instances. */
        CommandBufferRegistry commandBufferRegistry_{};

        /** @brief Unused; retained for interface uniformity. */
        CommandHandlerRegistry* commandHandlerRegistry_{nullptr};

        /** @brief Job system used by `flushParallel()` for concurrent buffer execution. */
        JobSystem& jobSystem_;

        /**
         * @brief Maps component type IDs to the `CommandBufferTypeId`s of their associated buffers.
         *
         * Indexed by component type ID; each slot holds one or more buffer IDs that can be
         * flushed independently in parallel.
         */
        std::vector<std::vector<CommandBufferTypeId>> componentToBufferGroups_{};

        /** @brief Ordered list of component type IDs that have at least one registered buffer. */
        std::vector<std::size_t> bufferGroupIndices_;

        /** @brief Module-scoped logger. */
        static inline auto& logger_ = helios::engine::util::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);

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

            /** @brief Unused; retained for interface uniformity. */
            CommandHandlerRegistry* commandHandlerRegistry_{nullptr};

            /** @brief Buffered commands pending application. */
            std::vector<TCommandType> commands_;

            /** @brief Reference to the entity manager mutations are applied to. */
            TEntityManager& entityManager_;

            public:
            /** @brief Role tag identifying this as a command buffer in the engine registry. */
            using EngineRoleTag = CommandBufferRole;

            /** @brief The command type stored in this buffer. */
            using Command_type = TCommandType;

            /** @brief Component type targeted by the buffered commands. */
            using Component_type = typename TCommandType::Component_type;

            /** @brief Reserves default capacity for the command vector. */
            InternalBuffer(TEntityManager& entityManager) : entityManager_(entityManager) {
                commands_.reserve(DEFAULT_ENTITY_MUTATION_COMMAND_BUFFER_CAPACITY);
            }

            /**
             * @brief Applies all buffered add/remove-component commands directly via the entity manager.
             *
             * Invalid entity handles are silently skipped.
             *
             * @param updateContext Frame-local ECS context (currently unused; kept for interface uniformity).
             */
            void flush(UpdateContext& updateContext)
            requires IsAddComponentCommand_v<TCommandType> || IsRemoveComponentCommand_v<TCommandType> {

                using Component_type = typename TCommandType::Component_type;

                logger_.info("Processing {0} commands", commands_.size());

                for (auto& command : commands_) {
                    if (!entityManager_.isValid(command.handle)) {
                        continue;
                    }
                    if constexpr (IsAddComponentCommand_v<TCommandType>) {
                        entityManager_.template emplace<Component_type>(command.handle, std::move(command.component));
                    } else {
                        std::ignore = entityManager_.template remove<Component_type>(command.handle);
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

            /**
             * @brief Moves all commands from `incoming` into this buffer.
             *
             * If the buffer is empty the vectors are swapped (zero-copy);
             * otherwise `incoming` is appended via move iterators, the cleared.
             *
             *
             * @param incoming Source vector; left in a valid but unspecified state after the call.
             */
            void add(std::vector<TCommandType>& incoming) {

                if (commands_.empty()) {
                     commands_.swap(incoming);
                } else {
                    commands_.reserve(incoming.size() + commands_.size());
                    commands_.insert(
                        commands_.end(),
                        std::make_move_iterator(incoming.begin()),
                        std::make_move_iterator(incoming.end())
                    );
                    incoming.clear();
                }
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
                        CommandBuffer(InternalBuffer<TCommand>{entityManager_})
                    );

                using Component_type = typename TCommand::Component_type;
                const auto cv = ComponentTypeId<THandle>::template id<Component_type>().value();

                if (componentToBufferGroups_.size() <= cv) {
                    componentToBufferGroups_.resize(cv + 1);
                }

                if (componentToBufferGroups_[cv].empty()) {
                    bufferGroupIndices_.push_back(cv);
                }
                componentToBufferGroups_[cv].push_back(CommandBufferTypeId::template id<InternalBuffer<TCommand>>());

                std::ignore = entityManager_.template  ensureSparseSet<Component_type>();

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
         * @brief Constructs the manager bound to `entityManager` and `jobSystem`.
         *
         * @param entity_manager Entity manager mutations are applied to.
         * @param jobSystem      Job system used for parallel flush execution.
         */
        explicit EntityMutationManager(TEntityManager& entity_manager, JobSystem& jobSystem)
        : entityManager_(entity_manager), jobSystem_(jobSystem) {}

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
         * @brief Accepts a batch of commands and enqueues them all at once.
         *
         * Delegates to `InternalBuffer::add(vector)` which swaps or appends
         * depending on whether the buffer is currently empty. The origin vector is
         * cleared afterwards.
         *
         * @tparam TCommand Deduced ECS command type. `TCommand::Handle_type` must match `THandle`.
         * @param  commands Source vector forwarded to the internal buffer.
         * @return `true` unconditionally (reserved for future error reporting).
         */
        template<typename TCommand>
        requires std::is_same_v<typename TCommand::Handle_type, THandle>
        bool submitBatch(std::vector<TCommand>& commands) {

            auto* model = modelFor<TCommand>();
            model->add(commands);

            return true;
        }

        /**
         * @brief Initialises the manager. Currently a no-op.
         *
         * @param commandHandlerRegistry Unused; kept for interface uniformity.
         */
        void init(CommandHandlerRegistry& commandHandlerRegistry) {}

        /**
         * @brief Flushes all internal buffers sequentially, applying every queued mutation.
         *
         * @param updateContext Frame-local ECS context forwarded to each buffer's flush.
         */
        void flush(UpdateContext& updateContext) {
            for (auto* buffer : commandBufferRegistry_.items()) {
                buffer->flush(updateContext);
            }

            entityManager_.finalizeMutations();
        }

        /**
         * @brief Flushes independent buffer groups concurrently via the `JobSystem`.
         *
         * Each component-type group is dispatched as a separate job; groups that operate
         * on different component types are assumed to be executable in parallel without contention.
         *
         * @param updateContext Frame-local ECS context forwarded to each buffer's flush.
         */
        void flushParallel(UpdateContext& updateContext) {

            std::vector<std::size_t> activeIndices;
            for (const auto idx : bufferGroupIndices_) {
                if (!componentToBufferGroups_[idx].empty()) {
                    activeIndices.push_back(idx);
                }
            }
            jobSystem_.runAndWait(activeIndices.size(), [&](const std::size_t groupIndex) {
                for (const auto bufferTypeId  : componentToBufferGroups_[activeIndices[groupIndex]]) {
                    logger_.info("Processing MutationCommandBuffer {0}", bufferTypeId.value());
                    auto* buffer = commandBufferRegistry_.item(bufferTypeId);
                    buffer->flush(updateContext);
                }
                entityManager_.finalizeMutations(ComponentTypeId<THandle>{activeIndices[groupIndex]});
            });


        }


    };


}