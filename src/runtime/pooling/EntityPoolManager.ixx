/**
 * @file EntityPoolManager.ixx
 * @brief Manager for Entity pooling and lifecycle management.
 */
module;

#include <cassert>
#include <optional>
#include <exception>
#include <functional>


export module helios.engine.runtime.pooling.EntityPoolManager;

import helios.ecs;

import helios.engine.core.types;

import helios.engine.runtime.world.UpdateContext;

import helios.engine.runtime.pooling.types;
import helios.engine.runtime.pooling.commands;
import helios.engine.runtime.pooling.components;

import helios.engine.runtime.pooling.EntityPool;
import helios.engine.runtime.pooling.EntityPoolRegistry;

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
     * @brief Manages the lifecycle of `EntityPool` instances for a specific handle type.
     */
    template<typename THandle>
    class EntityPoolManager {
        
        /**
         * @brief Pending pool-creation commands, one vector per handle type.
         */
        std::vector<PrewarmEntityPoolCommand<THandle>> prewarmEntityPoolCommands_;

        std::vector<ReleaseEntityCommand<THandle>> releaseEntityCommands_;


        static inline auto& logger_ = helios::core::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);


        /**
         * @brief Processes all pending `PrefabComponentPoolCommand`s for `THandle`.
         */
        void processPrewarmEntityPoolCommands(
            EntityManager<THandle>& entityManager,
            EntityPoolRegistry& entityPoolRegistry) noexcept {

            auto& commands = prewarmEntityPoolCommands_;

            for (auto& command : commands) {

                auto key = command.entityPoolKey;


                auto* entityPool = entityPoolRegistry.item(key);

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

        void processReleaseEntityCommands(EntityManager<THandle>& entityManager, EntityPoolRegistry& entityPoolRegistry) noexcept {

            auto& commands = releaseEntityCommands_;

            for (auto& command : commands) {

                auto key = command.entityPoolKey;
                auto handle = command.entityHandle;

                auto* entityPool = entityPoolRegistry.item(key);

                if (!entityPool || !entityPool->isLocked()) {
                    logger_.error("Failed to retrieve entity pool, or pool is in invalid state.");
                    assert(false && "Entity pool could not be retrieved, or pool is in invalid state    .");
                    continue;
                }

                entityPool->template release<THandle>(handle);
                if (auto entity = entityManager.entity(handle)) {
                    entity->setActive(false);
                }
            }

            commands.clear();
        }


        bool releaseAll() {
            assert(false && "TBD");
            return false;
        }

    public:

        EntityPoolManager(const EntityPoolManager&) = delete;
        EntityPoolManager& operator=(const EntityPoolManager&) = delete;
        EntityPoolManager(EntityPoolManager&&) noexcept = default;
        EntityPoolManager& operator=(EntityPoolManager&&) noexcept = default;

        EntityPoolManager() = default;


        /**
         * @brief Processes all pending commands sequentially for every managed handle type.
         *
         * @param updateContext  Current frame update context (unused directly, passed for API symmetry).
         */
        bool executeCommands(EntityManager<THandle>& entityManager, EntityPoolRegistry& entityPoolRegistry) noexcept {

            processPrewarmEntityPoolCommands(entityManager, entityPoolRegistry);
            processReleaseEntityCommands(entityManager, entityPoolRegistry);

            return true;
        }

        bool submit(PrewarmEntityPoolCommand<THandle>&& prewarmEntityPoolCommand) noexcept {
            prewarmEntityPoolCommands_.emplace_back(std::move(prewarmEntityPoolCommand));
            return true;
        }

        bool submit(ReleaseEntityCommand<THandle>&& releaseEntityCommand) noexcept {
            releaseEntityCommands_.emplace_back(std::move(releaseEntityCommand));
            return true;
        }

        bool init(CommandHandlerRegistry& commandHandlerRegistry) noexcept {
            commandHandlerRegistry.template handleCommands<
               PrewarmEntityPoolCommand<THandle>,
               ReleaseEntityCommand<THandle>
           >(*this);

            return true;
        };

        /**
         * @brief Resets this manager and releases all active entities back to their respective pools for every handle type.
         */
        void reset() {
            if (!releaseAll()) {
                std::terminate();
            }
        }

    };

}
