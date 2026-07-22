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

export module helios.engine.runtime.gameloop:Pass;

import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.SystemRegistry;
import helios.engine.runtime.world.System;
import helios.engine.runtime.world.concepts;
import helios.engine.runtime.world.types;

import helios.engine.runtime.messaging.command.types;
import helios.engine.runtime.messaging.command.concepts.IsCommandBufferLike;

import helios.engine.runtime.world.UpdateContext;

import helios.engine.runtime.enginestate.types;

using namespace helios::engine::runtime::messaging::command::types;
using namespace helios::engine::runtime::messaging::command::concepts;
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

    protected:
        /**
         * @brief Registry holding all systems for this pass.
         */
        helios::engine::runtime::world::SystemRegistry systemRegistry_{};

        /**
         * @brief Queue of system type IDs for execution order.
         *
         * Each entry holds a group of one or more SystemType-ids, whereas multiple ids indicate systems that can be executed in parallel.
         */
        std::vector<std::vector<SystemTypeId>> systemTypeIdQueue_;

        /**
         * @brief Reference to the owning GameWorld.
         *
         * @details Used to resolve optional command buffer dependencies for
         * systems declaring `CommandBuffer_type`.
         */
        GameWorld& gameWorld_;

        /**
         * @brief List of ManagerTypeIds.
         */
        std::vector<ManagerTypeId> managerTypeIds_;


        /**
         * @brief Registers the ManagerTypeIds for the Managers this pass should flush.
         *
         * @tparam T The type of the manager to register.
         */
        template<typename T>
        void registerManagerFlush() {
            T* manager = gameWorld_.tryManager<T>();
            assert(manager && "Manager buffer not found for system's manager");
            managerTypeIds_.push_back(ManagerTypeId::template id<T>());
        }

        /**
         * @brief Registers a callable system for this pass.
         *
         * @tparam TSystem The type of the system to register.
         * @param system The system instance to register.
         * @param isParallel
         * @return Reference to this pass.
         */
        template<typename TSystem>
        requires helios::engine::runtime::world::concepts::IsCallableSystemLike<std::remove_cvref_t<TSystem>>
        Pass& registerCallableSystem(TSystem&& system, const bool isParallel = false) {

            using SystemType = std::remove_cvref_t<TSystem>;

            if (!isParallel) {
                void* bufferPtr = nullptr;
                if constexpr (requires { typename SystemType::CommandBuffer_type; }) {
                    using TCommandBuffer = typename TSystem::CommandBuffer_type;
                    bufferPtr = gameWorld_.tryCommandBuffer<TCommandBuffer>();
                    assert(bufferPtr && "Command buffer not found for system's CommandBuffer_type");
                }

                systemRegistry_.template add<SystemType>(
                    System(std::forward<TSystem>(system), bufferPtr)
                );

            } else {
                if constexpr (requires { typename SystemType::CommandBuffer_type; }) {
                    using TCommandBuffer = typename TSystem::CommandBuffer_type;
                    systemRegistry_.template add<SystemType>(
                        System(std::forward<TSystem>(system), TCommandBuffer{})
                    );
                } else {
                    systemRegistry_.template add<SystemType>(
                        System(std::forward<TSystem>(system))
                    );
                }

            }

            return *this;
        }

        template<typename T, typename... Args>
        requires helios::engine::runtime::world::concepts::IsTypedSystemLike<T>
        Pass& registerParallelTypedSystem(Args&&... args) {

            T concreteSystem(std::forward<Args>(args)...);

            if constexpr (requires { typename T::CommandBuffer_type; }) {
                using TCommandBuffer = typename T::CommandBuffer_type;
                systemRegistry_.template add<T>(
                    System(std::move(concreteSystem), TCommandBuffer())
                );
            } else {
                systemRegistry_.template add<T>(
                    System(std::move(concreteSystem))
                );
            }

            return *this;
        }

        /**
         * @brief Registers a TypedSystem-like system with this pass.
         *
         * @tparam T The concrete system type to register.
         * @tparam Args The constructor arg types, if any.
         * @param args Concrete constructor args.
         *
         * @return This pass.
         */
        template<typename T, typename... Args>
        requires helios::engine::runtime::world::concepts::IsTypedSystemLike<T>
        Pass& registerTypedSystem(Args&&... args) {

            T concreteSystem(std::forward<Args>(args)...);

            void* bufferPtr = nullptr;
            if constexpr (requires { typename T::CommandBuffer_type; }) {
                using TCommandBuffer = typename T::CommandBuffer_type;
                bufferPtr = gameWorld_.tryCommandBuffer<TCommandBuffer>();
                assert(bufferPtr && "Command buffer not found for system's CommandBuffer_type");
            }

            systemRegistry_.template add<T>(
                System(std::move(concreteSystem), bufferPtr)
            );

            return *this;
        }

        /**
         * @brief Registers a system described by a `TypedSystemSpec` with this pass.
         *
         * Unpacks the argument tuple stored in `spec` and delegates to
         * `registerTypedSystem<TSystem>(args...)`.
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

    public:
        virtual ~Pass() = default;

        /**
         * @brief Constructs a pass bound to a specific GameWorld.
         *
         * @param gameWorld GameWorld used for system initialization and
         *        command buffer lookup.
         */
        explicit Pass(GameWorld& gameWorld) : gameWorld_(gameWorld) {};

        /**
         * @brief Updates all systems in this pass.
         *
         * @param updateContext The current update context.
         */
        virtual void update(helios::engine::runtime::world::UpdateContext& updateContext) = 0;

        /**
         * @brief Initializes all systems in this pass.
         *
         * @param gameWorld Reference to the game world.
         */
        virtual void init(helios::engine::runtime::world::GameWorld& gameWorld) = 0;

        /**
         * @brief Ends this pass.
         *
         * @return Reference to the owning phase.
         */
        virtual Phase& endPass() = 0;


        /**
         * @brief Determines if this pass should execute.
         *
         * @param updateContext The current update context.
         *
         * @return True if the pass should run.
         */
        virtual bool shouldRun(helios::engine::runtime::world::UpdateContext& updateContext) const noexcept = 0;

        /**
         * @brief Adds a system of type T to this pass.
         *
         * @tparam T The system type to add.
         * @tparam Args Constructor argument types for the system.
         *
         * @param args Arguments forwarded to the system constructor.
         *
         * @details If `T` defines `CommandBuffer_type`, the buffer is resolved
         * from the bound `GameWorld` and injected into the wrapped
         * `helios::engine::runtime::world::System`.
         *
         * @return Reference to this Pass for method chaining.
         */
        template<typename T, typename... Args>
        requires helios::engine::runtime::world::concepts::IsTypedSystemLike<T>
        Pass& addSystem(Args&&... args) {

            registerTypedSystem<T>(std::forward<Args>(args)...);

            systemTypeIdQueue_.push_back({SystemTypeId::template id<T>()});

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
        requires (helios::engine::runtime::world::concepts::IsTypedSystemLike<TSystem> && ...)
                && (sizeof...(TSystem) >= 2)
        Pass& addParallelSystems() {

            (registerTypedSystem<TSystem>(), ...);

            auto& group = systemTypeIdQueue_.emplace_back();
            group.reserve(sizeof...(TSystem));
            (group.push_back({SystemTypeId::template id<TSystem>()}), ...);

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
        requires (helios::engine::runtime::world::concepts::IsTypedSystemLike<
                typename std::remove_cvref_t<TSystem>::System_type
                > && ...)
                && (sizeof...(TSystem) >= 2)
        Pass& addParallelSystems(TSystem&&... system) {

            (registerParallelTypedSystemSpec<TSystem>(std::forward<TSystem>(system)), ...);

            auto& group = systemTypeIdQueue_.emplace_back();
            group.reserve(sizeof...(TSystem));
            (group.push_back({SystemTypeId::template id<typename std::remove_cvref_t<TSystem>::System_type>()}), ...);

            return *this;
        }

        /**
         * @brief Adds a system of type TSystem to this pass.
         *
         * @tparam TSystem The system type to add.
         *
         * @param system System forwarded to the system constructor.
         *
         * @details If `TSystem` defines `CommandBuffer_type`, the buffer is resolved
         * from the bound `GameWorld` and injected into the wrapped
         * `helios::engine::runtime::world::System`.
         *
         * @return Reference to this Pass for method chaining.
         */
        template<typename TSystem>
        requires helios::engine::runtime::world::concepts::IsCallableSystemLike<std::remove_cvref_t<TSystem>>
        Pass& addSystem(TSystem&& system) {
            registerCallableSystem(std::forward<TSystem>(system));
            systemTypeIdQueue_.push_back({SystemTypeId::template id<TSystem>()});
            return *this;
        }

        /**
         * @brief Adds multiple systems to this pass, allowing for parallel execution.
         *
         * @tparam TSystem The types of the systems to add.
         * @param system The system instances to add.
         * @return Reference to this pass.
         */
        template<typename ... TSystem>
        requires (helios::engine::runtime::world::concepts::IsCallableSystemLike<std::remove_cvref_t<TSystem>> && ...)
           && (sizeof...(TSystem) >= 2)
        Pass& addParallelSystems(TSystem&&... system) {

            (registerCallableSystem(std::forward<TSystem>(system), true), ...);

            auto& group = systemTypeIdQueue_.emplace_back();
            group.reserve(sizeof...(TSystem));
            (group.push_back({SystemTypeId::template id<std::remove_cvref_t<TSystem>>()}), ...);
            
            return *this;
        }

        /**
         * @brief  Registers the Managers this pass should flush.
         *
         * @tparam T The types of the Managers to flush.
         * @return Reference to this Pass for method chaining.
         */
        template<typename... T>
        requires (IsManagerLike<T> && ...)
        Pass& flush() {

            (registerManagerFlush<T>(), ...);

            return *this;
        }

        /**
         * @brief Returns a span of the ManagerTypeIds this pass is flushing.
         *
         * @return A span of ManagerTypeIds.
         */
        [[nodiscard]] std::span<const ManagerTypeId> managerTypeIds() noexcept {
            return managerTypeIds_;
        }

    };

}