/**
 * @file Pass.ixx
 * @brief Abstract base class for game loop passes.
 */
module;

#include <type_traits>
#include <utility>
#include <memory>
#include <cassert>
#include <vector>
#include <functional>
#include <iostream>
#include <exception>
#include "helios-engine-config.h"

export module helios.engine.runtime.gameloop:Pass;

import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.concepts;
import helios.core.common.concepts;
import helios.core.thread.JobSystem;

import helios.ecs.common.types;
import helios.ecs.common.concepts;
import helios.ecs.common.container;

import helios.ecs.manager;
import helios.ecs.system;
import helios.ecs.command;

import helios.engine.runtime.common.Session;

import helios.engine.runtime.world.UpdateContext;

import helios.engine.runtime.enginestate.types;

using namespace helios::ecs::common::types;
using namespace helios::ecs::common::concepts;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::world::concepts;
export namespace helios::engine::runtime::gameloop {

    class Phase;

    /**
     * @brief Abstract base class for game loop passes.
     *
     * @details A Pass represents a logical grouping of systems executed
     * sequentially within a Phase. Concrete implementations (TypedPass)
     * add state-based filtering via shouldRun().
     *
     * ## Key Features
     *
     * - **System Registration:** Systems are added via addSystem<T>()
     * - **Commit Points:** Control when events/commands are synchronized
     * - **State Filtering:** Passes can be skipped based on game state
     *
     * @see TypedPass
     * @see Phase
     * @see System
     */
    class Pass {

        friend class helios::engine::runtime::gameloop::Phase;

        using EcsDataContainer = ecs::common::container::EcsDataContainer;
        using JobSystem = helios::core::thread::JobSystem;

    protected:
        /**
         * @brief Registry holding all systems for this pass.
         */
        ecs::system::SystemRegistry systemRegistry_{};

        /**
         * @brief Ordered queue of system type IDs that drives execution order within this pass.
         */
        std::vector<std::vector<std::vector<ecs::system::types::SystemTypeId>>> systemTypeIdQueue_;

        /**
         * @brief Reference to the owning GameWorld.
         */
        GameWorld& gameWorld_;


        /**
         * @brief List of ManagerTypeIds.
         */
        std::vector<ecs::manager::types::ManagerTypeId> managerTypeIds_;

        /**
         * @brief List of ManagerTypeIds for parallel execution.
         */
        std::vector<ecs::manager::types::ManagerTypeId> parallelManagerTypeIds_;

        /**
         * @brief Registers the ManagerTypeIds for the Managers this pass should flush.
         *
         * @tparam T The type of the manager to register.
         */
        template<typename T>
        void registerManagerExecuteCommands() {
            if (!gameWorld_.tryManager<T>()) {

                if constexpr (std::is_default_constructible_v<T>) {
                    gameWorld_.registerManager<T>(T{});
                } else {
                    #if HELIOS_DEBUG
                    std::cerr << "Manager not found for system's manager: " << typeid(T).name() << '\n';
                    assert(gameWorld_.template tryManager<T>() && "Manager not found for system's manager");
                    #endif
                    std::terminate();
                }
            }

            managerTypeIds_.push_back(ecs::manager::types::ManagerTypeId::template id<T>());
        }

        template<typename TSystem>
        Pass& registerCallOperatorSystem(TSystem&& system) {
            using SystemType = std::remove_cvref_t<TSystem>;
            systemRegistry_.template add<SystemType>(ecs::system::System(std::move(system)));
            return *this;
        }

        /**
         * @brief Registers a system instance for parallel typed systems.
         */
        template<typename TSystem>
        requires ecs::system::concepts::IsEcsSystemLike<std::remove_cvref_t<TSystem>>
        Pass& registerTypedSystemInstance(TSystem&& system) {
            using SystemType = std::remove_cvref_t<TSystem>;
            systemRegistry_.add<SystemType>(std::move(system));
            return *this;
        }

        /**
         * @brief Registers a system described by a `TypedSystemSpec` with this pass.
         *
         * @tparam T A `TypedSystemSpec` specialisation whose `System_type` satisfies
         *           `IsEcsSystemLike`.
         * @param spec Spec instance carrying the system type and its construction arguments.
         * @return Reference to this pass.
         */
        template<typename T>
        Pass& registerTypedSystemSpec(T&& spec) {

            using Spec = std::remove_cvref_t<T>;
            using TSystem = Spec::System_type;

            std::apply([this](auto&... args) {
                registerTypedSystem<TSystem>(args...);
            }, spec.args);

            return *this;
        }

        /**
         * @brief Called on pass end.
         * @param ecsDataContainer The ECS data container.
         */
        virtual void onPassEnd(EcsDataContainer& ecsDataContainer) = 0;

        /**
         * @brief Updates all systems in this pass.
         *
         * @param ecsDataContainer The map of results from the current frame's system executions.
         * @param jobSystem The job system used for parallel execution of systems.
         */
        virtual void update(EcsDataContainer& ecsDataContainer, JobSystem& jobSystem) = 0;


        /**
         * @brief Determines if this pass should execute.
         *
         * @param ecsDataContainer
         *
         * @return True if the pass should run.
         */
        virtual bool shouldRun(EcsDataContainer& ecsDataContainer, common::Session& session) const noexcept = 0;

        /**
         * @brief Returns a span of the ManagerTypeIds this pass is flushing.
         *
         * @return A span of ManagerTypeIds.
         */
        [[nodiscard]] std::span<const ecs::manager::types::ManagerTypeId> managerTypeIds() noexcept {
            return managerTypeIds_;
        }


    public:

        using RunCondition = std::function<bool(helios::engine::runtime::world::UpdateContext&)>;


        virtual ~Pass() = default;

        /**
         * @brief Constructs a pass bound to a specific GameWorld.
         *
         * @param gameWorld GameWorld used for system initialization and
         *        command buffer lookup.
         */
        explicit Pass(GameWorld& gameWorld)
        : gameWorld_(gameWorld) {};


        /**
         * @brief Ends this pass.
         *
         * @return Reference to the owning phase.
         */
        virtual Phase& endPass() = 0;


        /**
         * @brief Specifies an additional condition to be considered with shouldRun().
         *
         * @param fn The conditional function to evaluate.
         *
         * @return Reference to this pass for method chaining.
         */
        virtual Pass& runIf(RunCondition fn) noexcept = 0;


        // +---------------------------------
        // | Typed/ Lambda Systems
        // +---------------------------------
        template<typename TSystem, typename ... TArgs>
        requires ecs::system::concepts::IsEcsSystemLike<TSystem>
        Pass& addSystem(TArgs&&... args) {
            using SystemType = std::remove_cvref_t<TSystem>;
            registerTypedSystemInstance<SystemType>(SystemType{std::forward<TArgs>(args) ...});
            systemTypeIdQueue_.push_back({{ecs::system::types::SystemTypeId::template id<SystemType>()}});
            return *this;
        }

        template<typename TSystem>
        requires ecs::system::concepts::IsEcsSystemLike<TSystem>
        Pass& addSystem() {
            using SystemType = std::remove_cvref_t<TSystem>;
            registerTypedSystemInstance<SystemType>(SystemType{});
            systemTypeIdQueue_.push_back({{ecs::system::types::SystemTypeId::template id<SystemType>()}});
            return *this;
        }

        template<typename TFuncSystem>
        requires ecs::system::concepts::IsCallableSystem<TFuncSystem>
        Pass& addSystem(TFuncSystem&& system) {
            using SystemType = std::remove_cvref_t<TFuncSystem>;
            registerCallOperatorSystem<SystemType>(std::forward<TFuncSystem>(system));
            systemTypeIdQueue_.push_back({{ecs::system::types::SystemTypeId::template id<SystemType>()}});
            return *this;
        }
        // +---------------------------------
        // +---------------------------------

        /**
         * @brief Adds multiple systems to this pass, allowing for parallel execution.
         *
         * @tparam TSystem The types of the systems to add.
         * @param system The system instances to add.
         * @return Reference to this pass.
         */
        template<typename ... TSystem>
        requires (ecs::system::concepts::IsCallableSystem<std::remove_cvref_t<TSystem>> && ...)
           && (sizeof...(TSystem) >= 2)
        Pass& addParallelSystems(TSystem&&... system) {
            (registerCallableSystem(std::forward<TSystem>(system)), ...);

            auto& group = systemTypeIdQueue_.emplace_back();
            group.reserve(sizeof...(TSystem));
            (group.push_back({{ecs::system::types::SystemTypeId::template id<std::remove_cvref_t<TSystem>>()}}), ...);
            
            return *this;
        }

        /**
         * @brief Adds two or more `TypedSystemSpec`-wrapped systems that may execute in parallel.
         *
         * @tparam TSystem `TypedSystemSpec` specialisations whose `System_type` satisfies
         *                 `IsEcsSystemLike`. At least two types are required.
         * @param system   Spec instances forwarded to `registerTypedSystemSpec()`.
         * @return Reference to this pass for method chaining.
         */
        template<typename ...TSystem>
        requires (ecs::system::concepts::IsEcsSystemLike<std::remove_cvref_t<TSystem>> && ...)
                && (sizeof...(TSystem) >= 2)
        Pass& addParallelSystems(TSystem&&... system) {

            (registerParallelTypedSystemInstance<TSystem>(std::forward<TSystem>(system)), ...);

            auto& group = systemTypeIdQueue_.emplace_back();
            group.reserve(sizeof...(TSystem));
            (group.push_back({{ecs::system::types::SystemTypeId::template id<std::remove_cvref_t<TSystem>>()}}), ...);

            return *this;
        }

        /**
         * @brief Adds TypedSystem-like systems that should be executed parallel.
         *
         * @tparam TSystem The types of the systems to add.
         *
         * @return Reference to this Pass for method chaining.
         */
        template<typename ...TSystem>
         requires (ecs::system::concepts::IsEcsSystemLike<TSystem> && ...)
                 && (sizeof...(TSystem) >= 2)
         Pass& addParallelSystems() {

            (registerTypedSystemInstance<TSystem>(), ...);

            auto& group = systemTypeIdQueue_.emplace_back();
            group.reserve(sizeof...(TSystem));
            (group.push_back({{ecs::system::types::SystemTypeId::template id<TSystem>()}}), ...);

            return *this;
        }

        /**
         * @brief Adds one or more `Serial`-wrapped system groups that may execute in parallel.
         *
         * Each `Serial<S1, S2, …>` argument defines an ordered sub-group: its member systems
         * are registered and run sequentially relative to each other, while distinct `Serial`
         * arguments form independent parallel lanes that the scheduler may execute concurrently.
         *
         * Example:
         * ```cpp
         * pass.addParallelSystems<
         *     Serial<PhysicsUpdate, PhysicsCollision>,
         *     Serial<AudioUpdate>
         * >();
         * ```
         *
         * @tparam TSerials One or more `Serial<…>` specialisations satisfying `IsSerialLike`.
         *                  At least one type is required.
         * @return Reference to this pass for method chaining.
         */
        template <typename ... TSerials>
        requires (sizeof ...(TSerials) >= 1) && ((ecs::system::concepts::IsSerialLike<TSerials>) && ...)
        Pass& addParallelSystems() {

            auto& parallelGroup = systemTypeIdQueue_.emplace_back();
            parallelGroup.reserve(sizeof...(TSerials));

            auto addSystemInstance = [this]
            <typename TSystem>
            (auto& serialGroup) {
                registerTypedSystemInstance<TSystem>(TSystem{});
                serialGroup.push_back({{ecs::system::types::SystemTypeId::template id<TSystem>()}});
            };

            auto registerSystems = [this, &addSystemInstance]
            <typename ...TSystems>
            (ecs::system::concepts::Serial<TSystems...>, auto& serialGroup) {
                serialGroup.reserve(sizeof...(TSystems));
                (addSystemInstance.template operator()<TSystems>(serialGroup), ...);
            };

            (registerSystems( std::remove_cvref_t<TSerials>{}, parallelGroup.emplace_back()), ...);


            return *this;
        }

        /**
         * @brief  Registers the Managers this pass should flush.
         *
         * @tparam T The types of the Managers to flush.
         * @return Reference to this Pass for method chaining.
         */
        template<typename... T>
        requires (ecs::manager::concepts::IsManagerLike<T> && ...)
        Pass& executeCommands() {

            (registerManagerExecuteCommands<T>(), ...);

            return *this;
        }

    };

}