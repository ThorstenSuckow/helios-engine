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

import helios.engine.core.types;

import helios.engine.runtime.world.UpdateContext;

import helios.engine.runtime.pooling.types;
import helios.engine.runtime.pooling.commands;
import helios.engine.runtime.pooling.components;

import helios.engine.runtime.pooling.EntityPool;
import helios.engine.runtime.pooling.TypedEntityPoolRegistry;

import helios.core.thread;
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
    template<typename TEntityPoolRegistry>
    class EntityPoolManager;


    template<
        template<typename> typename TLookupStrategy,
        typename... TMemberHandles
    >
   class EntityPoolManager<TypedEntityPoolRegistry<TLookupStrategy, TMemberHandles...>> {
        /**
         * @brief Registry holding all managed pools, keyed by `EntityPoolKey`.
         */
        TypedEntityPoolRegistry<TLookupStrategy, TMemberHandles...>& entityPoolRegistry_;

        /**
         * @brief Pending pool-creation commands, one vector per handle type.
         */
        std::tuple<std::vector<PrewarmEntityPoolCommand<TMemberHandles>>...> prefabEntityPoolCommands_;

        std::tuple<std::vector<ReleaseEntityCommand<TMemberHandles>>...> releaseEntityCommands_;

        /**
         * @brief Reference to the engine world used for cloning prefab entities.
         */
        ecs::EcsWorld& ecsWorld_;

        /**
         * @brief Returns the mutable command queue for `THandle`.
         */
        template<typename THandle>
        std::vector<PrewarmEntityPoolCommand<THandle>>& prewarmEntityPoolCommands() noexcept {
            return std::get<std::vector<PrewarmEntityPoolCommand<THandle>>>(prefabEntityPoolCommands_);
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
        void processPrewarmEntityPoolCommands(EntityManager<THandle>& entityManager) noexcept {

            auto& commands = prewarmEntityPoolCommands<THandle>();

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
                entityPool->prewarm(entityManager);

                if (!entityManager.destroy(command.ownerHandle)) {
                    logger_.error("Failed to destroy owner handle.");
                    assert(false && "Owner handle could not be destroyed.");
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
                if (auto entity = ecsWorld_.find(handle)) {
                    entity->setActive(false);
                }
            }

            commands.clear();
        }

        /**
         * @brief Dispatches all command-processing steps for `THandle`.
         */
        template<typename THandle>
        void processCommandsForHandle(EntityManager<THandle>& entityManager) noexcept {
            processPrewarmEntityPoolCommands<THandle>(entityManager);
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
            entityPoolRegistry_.template forEach<THandle>([&ecsWorld = ecsWorld_](EntityPool<THandle>& entityPool) noexcept {
                for (auto entityHandle : entityPool.activeEntities()) {
                    entityPool.release(entityHandle);
                    if (auto go = ecsWorld.find(entityHandle)) {
                        go->setActive(false);
                    }
                }
            });
        }

    public:



        /**
         * @brief Constructs an `EntityPoolManager`.
         *
         * @param entityPoolRegistry
         * @param ecsWorld  Engine world used for cloning prefab entities.
         * @param jobSystem    Job system used by `flushParallel()`.
         */
        explicit EntityPoolManager(
        TypedEntityPoolRegistry<TLookupStrategy, TMemberHandles...>&  entityPoolRegistry,
        EcsWorld& ecsWorld, JobSystem& jobSystem)
        : entityPoolRegistry_(entityPoolRegistry), ecsWorld_(ecsWorld), jobSystem_(jobSystem) {}



        /**
         * @brief Processes all pending commands sequentially for every managed handle type.
         *
         * @param updateContext  Current frame update context (unused directly, passed for API symmetry).
         */
        bool executeCommands(EcsWorld& ecsWorld) noexcept {

            (processCommandsForHandle<TMemberHandles>(ecsWorld.entityManager<TMemberHandles>()),...);

            return true;
        }


        /**
         * @brief Processes pending commands for all handle types in parallel via the `JobSystem`.
         *
         * @details Spawns one job per handle type and waits for all to complete before returning.
         *
         * @param updateContext  Current frame update context (unused directly, passed for API symmetry).
         */
        bool executeCommandsParallel() noexcept {
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
        bool submit(PrewarmEntityPoolCommand<THandle>&& prewarmEntityPoolCommand) noexcept {
            prewarmEntityPoolCommands<THandle>().emplace_back(std::move(prewarmEntityPoolCommand));
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
        bool init(CommandHandlerRegistry& commandHandlerRegistry) noexcept {

            (commandHandlerRegistry.template handleCommands<
               PrewarmEntityPoolCommand<TMemberHandles>,
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
