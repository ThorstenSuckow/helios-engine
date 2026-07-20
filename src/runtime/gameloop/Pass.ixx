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
         * @brief Reference to the owning GameWorld.
         *
         * @details Used to resolve optional command buffer dependencies for
         * systems declaring `CommandBuffer_type`.
         */
        GameWorld& gameWorld_;

        /**
         * @brief List of CommandBufferTypeIds.
         */
        std::vector<CommandBufferTypeId> commandBufferTypeIds_;

        /**
         * @brief List of ManagerTypeIds.
         */
        std::vector<ManagerTypeId> managerTypeIds_;

        /**
         * @brief Registers the CommandBufferTypeIds for the CommandBuffers this pass should submit.
         *
         * @tparam T The type of the command buffer to register.
         */
        template<typename T>
        void registerCommandBufferSubmit() {
            T* buffer = gameWorld_.tryCommandBuffer<T>();
            assert(buffer && "Command buffer not found for system's CommandBuffer");
            commandBufferTypeIds_.push_back(CommandBufferTypeId::template id<T>());
        }

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

            using SystemType = std::remove_cvref_t<TSystem>;

            void* bufferPtr = nullptr;
            if constexpr (requires { typename SystemType::CommandBuffer_type; }) {
                using TCommandBuffer = typename TSystem::CommandBuffer_type;
                bufferPtr = gameWorld_.tryCommandBuffer<TCommandBuffer>();
                assert(bufferPtr && "Command buffer not found for system's CommandBuffer_type");
            }

            systemRegistry_.template add<SystemType>(
                System(std::forward<TSystem>(system), bufferPtr)
            );

            return *this;
        }

        /**
         * @brief Registers the CommandBuffers this pass should submit to.
         *
         * @tparam T The types of the CommandBuffers to submit.
         * @return Reference to this Pass for method chaining.
         */
        template<typename... T>
        requires (IsCommandBufferLike<T> && ...)
        Pass& submit() {

            (registerCommandBufferSubmit<T>(), ...);

            return *this;
        }

        /**
         * @brief Returns a span of the CommandBufferTypeIds this pass is submitting to.
         *
         * @return A span of CommandBufferTypeIds.
         */
        [[nodiscard]] std::span<const CommandBufferTypeId> commandBufferTypeIds() noexcept {
            return commandBufferTypeIds_;
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