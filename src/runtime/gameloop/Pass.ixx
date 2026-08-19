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
import helios.engine.runtime.world.ContextProvider;
import helios.engine.runtime.world.concepts;
import helios.engine.runtime.world.types;

import helios.ecs.common.types;
import helios.ecs.common.concepts;

import helios.ecs.manager;
import helios.ecs.system;
import helios.ecs.command;

import helios.engine.runtime.world.UpdateContext;

import helios.engine.runtime.enginestate.types;

using namespace helios::ecs::common::types;
using namespace helios::ecs::common::concepts;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::world::concepts;
using namespace helios::engine::runtime::world::types;
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

        ContextProvider& contextProvider_;

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
                #if HELIOS_DEBUG
                std::cerr << "Manager not found for system's manager: " << typeid(T).name() << '\n';
                assert(gameWorld_.template tryManager<T>() && "Manager not found for system's manager");
                #endif
                std::terminate();
            }

            managerTypeIds_.push_back(ecs::manager::types::ManagerTypeId::template id<T>());
        }

        /**
         * @brief Registers the ManagerTypeIds for the Managers this pass should flush in parallel.
         *
         * @tparam T The type of the manager to register.
         */
        template<typename T>
        requires ecs::manager::concepts::HasExecuteCommandsParallel<T>
        void registerManagerExecuteCommandsParallel() {
            assert(gameWorld_.template tryManager<T>() && "Manager not found for system's manager");
            parallelManagerTypeIds_.push_back(ecs::manager::types::ManagerTypeId::template id<T>());
        }


        template<typename TCommandBuffer, typename TSystem>
        Pass& registerTypedSystem(TSystem&& system) {
            using SystemType = std::remove_cvref_t<TSystem>;
            systemRegistry_.template add<SystemType>(
                ecs::system::System::make<TCommandBuffer, SystemUpdateContext, SystemType>(std::forward<TSystem>(system))
            );
            return *this;
        }

        template<typename TCommandBuffer, typename TSystem>
        Pass& registerTypedSystem(TSystem&& system, TCommandBuffer& commandBuffer) {
            using SystemType = std::remove_cvref_t<TSystem>;
            using CommandBufferType = std::remove_cvref_t<TCommandBuffer>;
            systemRegistry_.template add<SystemType>(
                ecs::system::System::make<CommandBufferType, SystemUpdateContext, SystemType>(std::forward<TSystem>(system), commandBuffer)
            );
            return *this;
        }

        template<typename TCommandBuffer, typename TSystem>
        Pass& registerCallableSystem(TSystem&& system) {
            using SystemType = std::remove_cvref_t<TSystem>;
            systemRegistry_.template add<SystemType>(
                ecs::system::System::make<TCommandBuffer, SystemUpdateContext>(std::forward<TSystem>(system))
            );
            return *this;
        }

        template<typename TCommandBuffer, typename TSystem>
        Pass& registerCallableSystem(TSystem&& system, TCommandBuffer& commandBuffer) {
            using SystemType = std::remove_cvref_t<TSystem>;
            using CommandBufferType = std::remove_cvref_t<TCommandBuffer>;
            systemRegistry_.template add<SystemType>(
                ecs::system::System::make<CommandBufferType, SystemUpdateContext>(std::forward<TSystem>(system), commandBuffer)
            );
            return *this;
        }

        /**
         * @brief Registers a system instance for parallel typed systems.
         */
        template<typename TSystem>
        requires ecs::system::concepts::IsTypedSystemLike<std::remove_cvref_t<TSystem>>
        Pass& registerParallelTypedSystemInstance(TSystem&& system) {

            using SystemType = std::remove_cvref_t<TSystem>;

            using CommandBufferType = ecs::command::NullCommandBuffer;
            using UpdateContextType = runtime::world::types::SystemUpdateContext;

            systemRegistry_.add<SystemType>(
                ecs::system::System::make<CommandBufferType, UpdateContextType>(std::forward<TSystem>(system))
            );

            return *this;
        }

        /**
         * @brief Registers a system described by a `TypedSystemSpec` with this pass.
         *
         * @tparam T A `TypedSystemSpec` specialisation whose `System_type` satisfies
         *           `IsTypedSystemLike`.
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

        virtual void onPassEnd(helios::engine::runtime::world::UpdateContext& updateContext) = 0;

        /**
         * @brief Initializes all systems in this pass.
         */
        virtual void init() = 0;

        /**
         * @brief Updates all systems in this pass.
         *
         * @param updateContext The current update context.
         */
        virtual void update(helios::engine::runtime::world::UpdateContext& updateContext) = 0;


        /**
         * @brief Determines if this pass should execute.
         *
         * @param updateContext The current update context.
         *
         * @return True if the pass should run.
         */
        virtual bool shouldRun(helios::engine::runtime::world::UpdateContext& updateContext) const noexcept = 0;

        /**
         * @brief Returns a span of the ManagerTypeIds this pass is flushing.
         *
         * @return A span of ManagerTypeIds.
         */
        [[nodiscard]] std::span<const ecs::manager::types::ManagerTypeId> managerTypeIds() noexcept {
            return managerTypeIds_;
        }

        /**
         * @brief Returns a span of the ManagerTypeIds this pass is flushing in parallel.
         *
         * @return A span of ManagerTypeIds.
         */
        [[nodiscard]] std::span<const ecs::manager::types::ManagerTypeId> parallelManagerTypeIds() noexcept {
            return parallelManagerTypeIds_;
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
        explicit Pass(GameWorld& gameWorld, ContextProvider& contextProvider)
        : gameWorld_(gameWorld), contextProvider_(contextProvider) {};



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
        // | Typed Systems
        // +---------------------------------
        template<typename TSystem>
        requires ecs::system::concepts::IsTypedSystemLike<TSystem>
        Pass& addSystem(TSystem&& system) {
            using TCommandBuffer = ecs::command::NullCommandBuffer;
            registerTypedSystem<TCommandBuffer>(std::forward<TSystem>(system));
            systemTypeIdQueue_.push_back({{ecs::system::types::SystemTypeId::template id<TSystem>()}});
            return *this;
        }

        template<typename TCommandBuffer, typename TSystem>
        requires ecs::command::concepts::IsCommandBufferLike<TCommandBuffer>
                && ecs::system::concepts::IsTypedSystemLike<TSystem>
        Pass& addSystem(TSystem&& system) {
            registerTypedSystem<TCommandBuffer>(std::forward<TSystem>(system));
            systemTypeIdQueue_.push_back({{ecs::system::types::SystemTypeId::template id<TSystem>()}});
            return *this;
        }

        template<typename TCommandBuffer, typename TSystem>
        requires ecs::command::concepts::IsCommandBufferLike<TCommandBuffer>
                && ecs::system::concepts::IsTypedSystemLike<TSystem>
        Pass& addSystem(TSystem&& system, TCommandBuffer& commandBuffer) {
            registerTypedSystem<std::remove_cvref_t<TCommandBuffer>>(std::forward<TSystem>(system), commandBuffer);
            systemTypeIdQueue_.push_back({{ecs::system::types::SystemTypeId::template id<TSystem>()}});
            return *this;
        }
        // +---------------------------------
        // +---------------------------------

        // +---------------------------------
        // | Callable Systems
        // +---------------------------------
        template<typename TSystem>
        requires ecs::system::concepts::IsCallableSystemLike<std::remove_cvref_t<TSystem>>
        Pass& addSystem(TSystem&& system) {
            using TCommandBuffer = ecs::command::NullCommandBuffer;
            registerCallableSystem<TCommandBuffer>(std::forward<TSystem>(system));
            systemTypeIdQueue_.push_back({{ecs::system::types::SystemTypeId::template id<TSystem>()}});
            return *this;
        }

        template<typename TCommandBuffer, typename TSystem>
        requires ecs::system::concepts::IsCallableSystemLike<std::remove_cvref_t<TSystem>>
        Pass& addSystem(TSystem&& system) {
            registerCallableSystem<TCommandBuffer>(std::forward<TSystem>(system));
            systemTypeIdQueue_.push_back({{ecs::system::types::SystemTypeId::template id<TSystem>()}});
            return *this;
        }

        template<typename TCommandBuffer, typename TSystem>
        requires ecs::system::concepts::IsCallableSystemLike<std::remove_cvref_t<TSystem>>
        Pass& addSystem(TSystem&& system, TCommandBuffer& commandBuffer) {
            registerCallableSystem<std::remove_cvref_t<TCommandBuffer>>(std::forward<TSystem>(system), commandBuffer);
            systemTypeIdQueue_.push_back({{ecs::system::types::SystemTypeId::template id<TSystem>()}});
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
        requires (ecs::system::concepts::IsCallableSystemLike<std::remove_cvref_t<TSystem>> && ...)
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
         * Each spec is unpacked via `registerTypedSystemSpec()` and all resulting
         * system type IDs are placed in a single execution group, signalling to
         * the scheduler that the systems are independent and can run concurrently.
         *
         * ```cpp
         * pass.addParallelSystems(
         *     TypedSystem<PhysicsSystem>(gravity),
         *     TypedSystem<AudioSystem>(listener)
         * );
         * ```
         *
         * @tparam TSystem `TypedSystemSpec` specialisations whose `System_type` satisfies
         *                 `IsTypedSystemLike`. At least two types are required.
         * @param system   Spec instances forwarded to `registerTypedSystemSpec()`.
         * @return Reference to this pass for method chaining.
         */
        template<typename ...TSystem>
        requires (ecs::system::concepts::IsTypedSystemLike<std::remove_cvref_t<TSystem>> && ...)
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
         requires (ecs::system::concepts::IsTypedSystemLike<TSystem> && ...)
                 && (sizeof...(TSystem) >= 2)
         Pass& addParallelSystems() {

            (registerTypedSystem<TSystem>(), ...);

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
                registerParallelTypedSystemInstance<TSystem>(TSystem{});
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

        /**
         * @brief Register the Manager this pass should flush in parallel.
         *
         * @tparam TManager The type of the Manager to flush in parallel.
         * @return Reference to this Pass for method chaining.
         */
        template<typename TManager>
        requires (ecs::manager::concepts::IsManagerLike<TManager> && ecs::manager::concepts::HasExecuteCommandsParallel<TManager>)
        Pass& executeCommandsParallel() {

            registerManagerExecuteCommandsParallel<TManager>();

            return *this;
        }


    };

}