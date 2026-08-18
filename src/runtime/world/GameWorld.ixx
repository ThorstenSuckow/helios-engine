/**
 * @file GameWorld.ixx
 * @brief Central game state container for entities, resources, and the active level.
 */
module;

#include <cassert>
#include <format>
#include <helios-engine-config.h>
#include <memory>
#include <span>
#include <string>

export module helios.engine.runtime.world.GameWorld;

import helios.engine.runtime.world.Session;

import helios.core.thread.JobSystem;
import helios.core.container;

import helios.engine.runtime.world.RuntimeEnvironment;
import helios.engine.platform.environment.types;

import helios.ecs;

import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.GameObject;
import helios.engine.runtime.world.ContextProvider;
import helios.engine.runtime.world.types.GameObjectId;
import helios.engine.runtime.world.types.GameObjectHandle;

import helios.core.log.Logger;
import helios.core.log.LogManager;
import helios.engine.runtime.world.Level;

import helios.engine.runtime.world.concepts;

using namespace helios::core::thread;
using namespace helios::ecs::common::concepts;
using namespace helios::ecs;
using namespace helios::engine::runtime::world::concepts;
using namespace helios::engine::platform::environment::types;
using namespace helios::engine::runtime::world::types;
#define HELIOS_LOG_SCOPE "GameWorld"
export namespace helios::engine::runtime::world {

    /**
     * @brief Engine-level representation of a runtime world.
     *
     * Owns an EcsWorld and provides world-scoped resources and runtime state information.
     */
    class GameWorld {

        struct GameWorldResources{};


        /**
         * @brief The logger used with this GameWorld instance.
         *
         * Defaults to HELIOS_LOG_SCOPE.
         */
        inline static const helios::core::log::Logger& logger_ = helios::core::log::LogManager::loggerForScope(
            HELIOS_LOG_SCOPE);

        EcsWorld ecsWorld_;

        ecs::manager::ManagerRegistry managerRegistry_{};

        ecs::command::CommandHandlerRegistry commandHandlerRegistry_{};

        /**
         * @brief Session object storing runtime/game state.
         */
        Session session_;

        /**
         * @brief Runtime environment facade for platform readiness state.
         */
        RuntimeEnvironment runtimeEnvironment_;

        /**
         * @brief Reference to the job system used for parallel task execution.
         */
        JobSystem& jobSystem_;

        helios::core::container::TypeMap<GameWorldResources> resourceRegistry_;


    public:

        /**
         * @brief Constructs `GameWorld` and creates internal session/environment entities.
         */
        explicit GameWorld(EcsWorld&& ecsWorld, JobSystem& jobSystem)
        : ecsWorld_(std::move(ecsWorld)),
          session_(Session(ecsWorld_.add<GameObjectHandle>())),
          runtimeEnvironment_(RuntimeEnvironment(ecsWorld_.add<PlatformHandle>())),
          jobSystem_(jobSystem)
        {};

        /**
         * @brief Non-copyable, non-movable.
         */
        GameWorld(const GameWorld&) = delete;
        GameWorld& operator=(const GameWorld&) = delete;
        GameWorld(GameWorld&&) = default;
        GameWorld& operator=(GameWorld&&) = delete;

        [[nodiscard]] EcsWorld& ecsWorld() {
            return ecsWorld_;
        }

        /**
         * @brief Returns a reference to the current game session.
         *
         * @return Reference to the Session.
         */
        [[nodiscard]] Session& session() {
            return session_;
        }

        /**
         * @brief Returns a reference to the job system used for parallel task execution.
         *
         * @return Reference to the JobSystem.
         */
        [[nodiscard]] JobSystem& jobSystem() {
            return jobSystem_;
        }

        /**
         * @brief Returns a reference to the current runtime platform.
         *
         * @return Reference to the Platform.
         */
        [[nodiscard]] RuntimeEnvironment& runtimeEnvironment() {
            return runtimeEnvironment_;
        }

        template<typename TResource, typename ... TArgs>
        TResource& addResource(TArgs&&... args) {
            return resourceRegistry_.add<TResource>(std::forward<TArgs>(args)...);
        }


        template<typename TResource>
        TResource& resource() {
            return resourceRegistry_.get<TResource>();
        }

        /**
         * @brief Initializes managers and command buffers.
         */
        GameWorld& init(ContextProvider& contextProvider) {
            for (auto& manager : managerRegistry_.items()) {
                auto initContextTypeId = manager->expectedInitContextTypeId();

                auto contextRef = contextProvider.get(initContextTypeId);
                manager->init(contextRef);
            }

            return *this;
        }


        /**
         * @brief Checks whether a Manager of type T is registered.
         *
         * @tparam T The Manager type. Must satisfy IsManagerLike.
         *
         * @return True if the Manager is registered.
         */
        template<typename T>
        requires ecs::manager::concepts::IsManagerLike<T>
        [[nodiscard]] bool hasManager() const {
            return managerRegistry_.has<T>();
        }



        /**
         * @brief Registers and constructs a Manager of type T.
         *
         * @tparam T The Manager type. Must satisfy IsManagerLike.
         * @tparam Args Constructor argument types.
         *
         * @param args Arguments forwarded to the T constructor.
         *
         * @return Reference to the newly registered Manager.
         */
        template<typename T, typename... Args>
        requires helios::ecs::manager::concepts::IsManagerLike<T>
        T& registerManager(Args&&... args) {
            return managerRegistry_.add<T>(std::forward<Args>(args)...);
        }


        /**
         * @brief Retrieves a registered Manager by type, or nullptr if not found.
         *
         * @tparam T The Manager type. Must satisfy helios::ecs::manager::concepts::IsManagerLike.
         *
         * @return Pointer to the Manager, or nullptr if not registered.
         */
        template<typename T>
        requires helios::ecs::manager::concepts::IsManagerLike<T>
        T* tryManager() noexcept {
            return managerRegistry_.item<T>();
        }

        /**
         * @brief Retrieves a registered Manager by type, or nullptr if not found.
         *
         * @tparam T The Manager type. Must satisfy helios::ecs::manager::concepts::IsManagerLike.
         *
         * @return Const Pointer to the Manager, or nullptr if not registered.
         */
        template<typename T>
        requires helios::ecs::manager::concepts::IsManagerLike<T>
        const T* tryManager() const noexcept {
            return managerRegistry_.item<T>();
        }


        /**
         * @brief Returns a reference to the CommandHandlerRegistry.
         *
         * @return Reference to the CommandHandlerRegistry.
         */
        [[nodiscard]] ecs::command::CommandHandlerRegistry& commandHandlerRegistry() noexcept {
            return commandHandlerRegistry_;
        }

        /**
         * @brief Returns a reference to the ManagerRegistry.
         *
         * @return Reference to the ManagerRegistry.
         */
        [[nodiscard]] ecs::manager::ManagerRegistry& managerRegistry() noexcept {
            return managerRegistry_;
        }


        /**
         * @brief Resets all managers and the session to their initial state.
         *
         * @details Called during level transitions or game restarts to clear
         * accumulated state. Invokes reset() on all managers and the session.
         */
        void reset() {
            for (auto& mgr : managerRegistry_.items()) {
                mgr->reset();
            }
        }



        /**
         * @brief Builds a typed ECS view for a handle domain and component set.
         *
         * @tparam THandle Handle domain type.
         * @tparam Components Component types to include.
         *
         * @return Domain-specific view.
         */
        template <typename THandle, typename... Components>
        [[nodiscard]] auto view() {
            return ecsWorld_.view<THandle, Components...>();
        }

        /**
         * @brief Finds an entity facade by handle.
         *
         * @tparam THandle Handle type.
         *
         * @param handle Entity handle to resolve.
         *
         * @return Domain-specific entity facade (or empty facade if not found).
         */
        template<typename THandle>
        [[nodiscard]] auto find(const THandle handle) noexcept {
            return ecsWorld_.find<THandle>(handle);
        }

        /**
         * @brief Adds a new entity in the domain inferred from `THandle`.
         *
         * @tparam THandle Handle type.

         *
         * @return Domain-specific entity facade for the created entity.
         */
        template<typename THandle>
        [[nodiscard]] auto add(const bool isActive = true) noexcept {
            auto entity = ecsWorld_.add<THandle>();
            entity.setActive(isActive);
            return entity;
        }

        /**
         * @brief Destroys an entity in the domain inferred from `THandle`.
         *
         * @tparam THandle Handle type.
         *
         * @param handle Entity handle to destroy.
         *
         * @return Domain-specific destroy result.
         */
        template<typename THandle>
        [[nodiscard]] auto destroy(const THandle handle) noexcept {
            return ecsWorld_.destroy<THandle>(handle);
        }


        /**
         * @brief Returns direct access to the entity manager for a specific handle type.
         *
         * @tparam THandle Handle type.
         * @return Reference to the internal EntityManager for the specified handle type.
         */
        template<typename THandle>
        auto& entityManager() noexcept {
            return ecsWorld_.entityManager<THandle>();
        }

    };

}

