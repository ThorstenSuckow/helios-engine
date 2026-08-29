/**
 * @file UpdateContext.ixx
 * @brief Per-frame context passed to systems during game loop updates.
 */
module;

#include <span>
#include <cassert>

export module helios.engine.runtime.world.UpdateContext;

import helios.engine.input.InputSnapshot;

import helios.engine.runtime.world.Level;

import helios.engine.runtime.world.RuntimeEnvironment;
import helios.ecs.common.types;


import helios.ecs.EcsWorld;
import helios.ecs.View;


export namespace helios::engine::runtime::world {


    /**
     * @brief Per-frame context passed to systems during game loop updates.
     *
     * @details UpdateContext bundles frame-scoped data and services used by
     * system updates: timing values, immutable input/viewport snapshots,
     * session/runtime environment access, typed entity access via `EcsWorld`,
     * event-bus read/write channels, and typed ECS access.
     *
     * Command submission is handled by systems through injected command buffers
     * (`CommandBufferType`) using `cmdBuffer.template add<TCommand>(...)`.
     *
     * @see GameLoop
     * @see Session
     * @see RuntimeEnvironment
     * @see EcsWorld
     * @see ResourceRegistry
     */
    class UpdateContext {

    private:
        /**
         * @brief Time elapsed since the last frame, in seconds.
         */
        float deltaTime_ = 0.0f;

        /**
        * @brief Time elapsed since the first frame, in seconds.
         */
        float totalTime_ = 0.0f;

        /**
        * @brief The current frame count, since the GameLoop started.
         */
        std::size_t frameCount_ = 0;

        /**
         * @brief Immutable snapshot of input state for the current frame.
         */
        const helios::engine::input::InputSnapshot& inputSnapshot_;

        /**
         * @brief Reference to the current platform entity.
         */
        helios::engine::runtime::world::RuntimeEnvironment& runtimeEnvironment_;

        /**
         * @brief Pointer to the active Level, or nullptr if no level is loaded.
         */
        const Level* level_;

        /**
         * @brief Aggregate typed world used for domain-routed ECS operations.
         */
        helios::ecs::EcsWorld& ecsWorld_;
    public:


        /**
         * @brief Constructs an UpdateContext with all per-frame dependencies.
         *
         * @param session Reference to current session state.
         * @param runtimeEnvironment Reference to runtime-environment state.
         * @param deltaTime Time since last frame in seconds.
         * @param totalTime Accumulated time in seconds.
         * @param frameCount Accumulated frames.
         * @param phaseEventBus Phase-level event bus.
         * @param passEventBus Pass-level event bus.
         * @param frameEventBus Frame-level event bus.
         * @param inputSnapshot Immutable frame input snapshot.
         * @param level Active level pointer, or nullptr.
         * @param ecsWorld Aggregate typed world for entity operations.
         */
        UpdateContext(
            helios::engine::runtime::world::RuntimeEnvironment& runtimeEnvironment,
            const float deltaTime,
            const float totalTime,
            const std::size_t frameCount,
            const helios::engine::input::InputSnapshot& inputSnapshot,
            helios::ecs::EcsWorld& ecsWorld
        ) :
        runtimeEnvironment_(runtimeEnvironment),
        deltaTime_(deltaTime),
        totalTime_(totalTime),
        frameCount_(frameCount),
        inputSnapshot_(inputSnapshot),
        ecsWorld_(ecsWorld)
        {

        }



        /**
         * @brief Returns the time elapsed since the last frame, in seconds.
         *
         * @return Delta time in seconds.
         */
        [[nodiscard]] float deltaTime() const noexcept {
            return deltaTime_;
        }

        /**
         * @brief Returns the time elapsed since the first frame, in seconds.
         *
         * @return Total time in seconds.
         */
        [[nodiscard]] float totalTime() const noexcept {
            return totalTime_;
        }

        /**
         * @brief Returns the current frame count.
         *
         * @return Current frame count.
         */
        [[nodiscard]] std::size_t frameCount() const noexcept {
            return frameCount_;
        }

        /**
         * @brief Returns the immutable input snapshot for this frame.
         *
         * @return Const ref to the current InputSnapshot.
         */
        [[nodiscard]] const helios::engine::input::InputSnapshot& inputSnapshot() const noexcept {
            return inputSnapshot_;
        }

        /**
         * @brief Resolves an entity facade by typed handle.
         *
         * @tparam THandle Handle type.
         *
         * @param handle Entity handle to resolve.
         *
         * @return Domain-specific entity facade.
         */
        template<typename THandle>
        [[nodiscard]] auto find(const THandle handle) noexcept {
            return ecsWorld_.find<THandle>(handle);
        }


        /**
         * @brief Returns the active Level.
         *
         * @return Pointer to the Level, or nullptr if no level is loaded.
         */
        [[nodiscard]] const Level* level() noexcept {
            return level_;
        }


        /**
         * @brief Returns the runtime environment for platform/runtime readiness state.
         *
         * @return Reference to runtime environment.
         */
        [[nodiscard]] helios::engine::runtime::world::RuntimeEnvironment& runtimeEnvironment() const noexcept {
            return runtimeEnvironment_;
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
         * @brief Returns the sparse set for a handle domain and component type.
         *
         * @tparam THandle Handle domain type.
         * @tparam TComponent Component type.
         *
         * @return Pointer to the sparse set for the specified handle and component types.
         */
        template <typename THandle, typename TComponent>
        [[nodiscard]] const auto* sparseSet() const {
            return ecsWorld_.template sparseSet<THandle, TComponent>();
        }

        template <typename THandle>
        [[nodiscard]] auto& entityManager() {
            return ecsWorld_.template entityManager<THandle>();
        }
        /**
         * @brief Checks whether a handle refers to a valid entity in the appropriate sub-world.
         * @tparam THandle Handle domain type.
         * @param handle The handle to check.
         * @return True if the handle is valid, false otherwise.
         */
        template <typename THandle>
        [[nodiscard]] bool isValid(THandle handle) const {
            return ecsWorld_.template isValid<THandle>(handle);
        }

        /**
         * @brief Clears the specified dirty components for a handle domain and component set.
         *
         * @tparam THandle Handle domain type.
         * @tparam Components Component types to include.
         *
         * @return Domain-specific view.
         */
        template <typename THandle = void, typename... Components>
        void clearDirtySets() {
            ecsWorld_.clearDirtySets<THandle, Components...>();
        }

    };
}

