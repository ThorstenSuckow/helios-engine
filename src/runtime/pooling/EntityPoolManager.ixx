/**
 * @file EntityPoolManager.ixx
 * @brief Manager for Entity pooling and lifecycle management.
 */
module;

#include <cassert>
#include <optional>
#include <unordered_map>
#include <cstddef>
#include <functional>


export module helios.engine.runtime.pooling.EntityPoolManager;

import helios.ecs;
import helios.engine.runtime.pooling.types;

import helios.engine.runtime.world.UpdateContext;

import helios.engine.runtime.pooling.EntityPool;
import helios.engine.runtime.pooling.TypedEntityPoolRegistry;

import helios.engine.runtime.pooling.commands;
import helios.engine.runtime.pooling.components;

import helios.ecs.command;

import helios.core.thread;
import helios.engine.core.types;

import helios.core.log;

using namespace helios::engine::runtime::pooling::types;
using namespace helios::core::thread;
using namespace helios::engine::runtime::pooling::components;
using namespace helios::engine::runtime::pooling::commands;
using namespace helios::ecs;
using namespace helios::ecs::command;
using namespace helios::engine::runtime::world;


#define HELIOS_LOG_SCOPE "helios::engine::runtime::pooling::EntityPoolManager"
export namespace helios::engine::runtime::pooling {

    /**
     * @brief Manages the lifecycle of `EntityPool` instances for one or more handle types.
     *
     * @details Processes `PrefabComponentPoolCommand`s to clone prefab entities into pools,
     * locks the pools after population, and registers them in the internal
     * `TypedEntityPoolRegistry`. Supports sequential (`flush`) and parallel
     * (`flushParallel`) command processing.
     *
     * @tparam TMemberHandles  Pack of handle types whose pools are managed by this instance.
     */
    template<typename TEntityPoolRegistry, typename TInitContext, typename TExecutionContext>
    class EntityPoolManager;


    template<
        typename TInitContext,
        typename TExecutionContext,
        template<typename> typename TLookupStrategy,
        typename... TMemberHandles
    >
    requires ecs::common::concepts::ProvidesCommandHandlerRegistry<TInitContext, ecs::command::CommandHandlerRegistry>
   class EntityPoolManager<TypedEntityPoolRegistry<TLookupStrategy, TMemberHandles...>, TInitContext, TExecutionContext> {
        /**
         * @brief Registry holding all managed pools, keyed by `EntityPoolKey`.
         */
        TypedEntityPoolRegistry<TLookupStrategy, TMemberHandles...>& entityPoolRegistry_;

        /**
         * @brief Pending pool-creation commands, one vector per handle type.
         */
        std::tuple<std::vector<PrefabEntityPoolCommand<TMemberHandles>>...> prefabEntityPoolCommands_;

        std::tuple<std::vector<ReleaseEntityCommand<TMemberHandles>>...> releaseEntityCommands_;

        /**
         * @brief Reference to the engine world used for cloning prefab entities.
         */
        ecs::EntitySpace& entitySpace_;

        /**
         * @brief Returns the mutable command queue for `THandle`.
         */
        template<typename THandle>
        std::vector<PrefabEntityPoolCommand<THandle>>& prefabEntityPoolCommands() noexcept {
            return std::get<std::vector<PrefabEntityPoolCommand<THandle>>>(prefabEntityPoolCommands_);
        }

        template<typename THandle>
        std::vector<ReleaseEntityCommand<THandle>>& releaseEntityCommands() noexcept {
            return std::get<std::vector<ReleaseEntityCommand<THandle>>>(releaseEntityCommands_);
        }

        static inline auto& logger_ = helios::core::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);

        /**
         * @brief Job system used for parallel command processing in `flushParallel()`.
         */
        JobSystem& jobSystem_;


        /**
         * @brief Processes all pending `PrefabComponentPoolCommand`s for `THandle`.
         *
         * @details For each command, clones the prefab entity `command.amount` times,
         * marks the clones inactive, populates an `EntityPool`, locks it, and registers
         * it in `entityPoolRegistry_`. Clears the command queue on completion.
         */
        template<typename THandle>
        void processPrefabEntityPoolCommands() noexcept {

            auto& commands = prefabEntityPoolCommands<THandle>();

            for (auto& command : commands) {

                auto key = command.entityPoolKey;

                if (!key.isValid()) {
                    assert(false && "Invalid key passed with command.");
                    logger_.error("Invalid key passed with command.");
                    continue;
                }

                auto* entityPool = entityPoolRegistry_.template pool<THandle>(key);

                if (!entityPool) {
                    logger_.error("Failed to retrieve entity pool.");
                    assert(false && "Entity pool could not be retrieved.");
                    continue;
                }

                if (entityPool->isLocked()) {
                    logger_.error("Entity pool is already locked.");
                    assert(false && "Entity pool is already locked.");
                    continue;
                }

                entityPool->setPoolSize(command.amount);

                const size_t used  = entityPool->activeCount() + entityPool->inactiveCount();
                const size_t space = used < entityPool->size() ? entityPool->size() - used : 0;
                const auto prefabHandle = command.prefabHandle;

                auto source = entitySpace_.findEntity(prefabHandle);
                source->template remove<PrefabEntityPoolRequestComponent<THandle>>();

                for (size_t i = 0; i < space; i++) {
                    auto go = entitySpace_.copyEntity(prefabHandle);
                    go.setActive(false);
                    entityPool->addInactive(go.handle());
                }

                if (!entityPool->lock()) {
                    logger_.error("Failed to lock entity pool.");
                    assert(false && "Entity pool could not be locked.");
                    continue;
                }

            }

            commands.clear();
        }

        template<typename THandle>
        void processReleaseEntityCommands() noexcept {

            auto& commands = releaseEntityCommands<THandle>();

            for (auto& command : commands) {

                auto key = command.entityPoolKey;
                auto handle = command.entityHandle;

                auto* entityPool = entityPoolRegistry_.template pool<THandle>(key);

                if (!entityPool || !entityPool->isLocked()) {
                    logger_.error("Failed to retrieve entity pool, or pool is in invalid state.");
                    assert(false && "Entity pool could not be retrieved, or pool is in invalid state    .");
                    continue;
                }

                entityPool->release(handle);
                if (auto entity = entitySpace_.findEntity(handle)) {
                    entity->setActive(false);
                }
            }

            commands.clear();
        }

        /**
         * @brief Dispatches all command-processing steps for `THandle`.
         */
        template<typename THandle>
        void processCommandsForHandle() noexcept {
            processPrefabEntityPoolCommands<THandle>();
            processReleaseEntityCommands<THandle>();
        }

        /**
        * @brief Resets this manager and releases all active entities back to their pool for `THandle`.
        *
        * @details Iterates over every pool registered for `THandle`, calls `release()` on each
        * active entity, and deactivates it in the engine world.
        *
        * @tparam THandle  Handle type whose pools should be reset.
        */
        template<typename THandle>
        void releaseAll() {
            entityPoolRegistry_.template forEach<THandle>([&entitySpace = entitySpace_](EntityPool<THandle>& entityPool) {
                for (auto entityHandle : entityPool.activeEntities()) {
                    entityPool.release(entityHandle);
                    if (auto go = entitySpace.findEntity(entityHandle)) {
                        go->setActive(false);
                    }
                }
            });
        }

    public:

        /** @brief Engine role tag identifying this class as a manager. */
        using EcsRoleTag = ecs::manager::tags::ManagerRole;

        using ExecutionContextType = TExecutionContext;
        using InitContextType = TInitContext;

        /**
         * @brief Constructs an `EntityPoolManager`.
         *
         * @param entityPoolRegistry
         * @param entitySpace  Engine world used for cloning prefab entities.
         * @param jobSystem    Job system used by `flushParallel()`.
         */
        explicit EntityPoolManager(
        TypedEntityPoolRegistry<TLookupStrategy, TMemberHandles...>&  entityPoolRegistry,
        EntitySpace& entitySpace, JobSystem& jobSystem)
        : entityPoolRegistry_(entityPoolRegistry), entitySpace_(entitySpace), jobSystem_(jobSystem) {}



        /**
         * @brief Processes all pending commands sequentially for every managed handle type.
         *
         * @param updateContext  Current frame update context (unused directly, passed for API symmetry).
         */
        bool executeCommands(TExecutionContext&) noexcept {

            (processCommandsForHandle<TMemberHandles>(),...);

            return true;
        }


        /**
         * @brief Processes pending commands for all handle types in parallel via the `JobSystem`.
         *
         * @details Spawns one job per handle type and waits for all to complete before returning.
         *
         * @param updateContext  Current frame update context (unused directly, passed for API symmetry).
         */
        bool executeCommandsParallel(TExecutionContext&) noexcept {
            std::array<std::function<void()>, sizeof ...(TMemberHandles)> jobs{
                [this]() {
                    this->template processCommandsForHandle<TMemberHandles>();
                }...
            };

            jobSystem_.runAndWait(jobs.size(), [&jobs](const std::size_t jobsIndex) {
                jobs[jobsIndex]();
            });

            return true;
        }


        /**
         * @brief Enqueues a pool-creation command for deferred processing.
         *
         * @details The command will be consumed during the next `flush()` or `flushParallel()` call.
         *
         * @tparam THandle                   Handle type of the pool to create.
         * @param prefabComponentPoolCommand  Command describing the prefab and pool size.
         *
         * @return `true` (always; reserved for future error propagation).
         */
        template<typename THandle>
        bool submit(PrefabEntityPoolCommand<THandle>&& prefabEntityPoolCommand) noexcept {
            prefabEntityPoolCommands<THandle>().emplace_back(std::move(prefabEntityPoolCommand));
            return true;
        }

        template<typename THandle>
        bool submit(ReleaseEntityCommand<THandle>&& releaseEntityCommand) noexcept {
            releaseEntityCommands<THandle>().emplace_back(std::move(releaseEntityCommand));
            return true;
        }

        /**
         * @brief Registers command handlers for all managed handle types in the given registry.
         *
         * @details Must be called once during engine initialisation so that
         * `PrefabComponentPoolCommand`s are routed to this manager.
         *
         * @param commandHandlerRegistry  Registry to register handlers with.
         */
        bool init(TInitContext& initContext) noexcept {

            auto& commandHandlerRegistry = initContext.commandHandlerRegistry();

            (commandHandlerRegistry.template handleCommands<
               PrefabEntityPoolCommand<TMemberHandles>,
               ReleaseEntityCommand<TMemberHandles>
           >(*this), ...);

            return true;
        };

        /**
         * @brief Resets this manager and releases all active entities back to their respective pools for every handle type.
         */
        void reset() {
            (releaseAll<TMemberHandles>(), ...);
        }

    };

}
