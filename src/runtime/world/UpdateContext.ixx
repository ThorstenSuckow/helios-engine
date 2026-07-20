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
import helios.engine.runtime.messaging.event.GameLoopEventBus;

import helios.engine.runtime.world.RuntimeEnvironment;
import helios.ecs.types.EntityHandle;

import helios.ecs.View;
import helios.engine.runtime.world.Session;


import helios.engine.runtime.world.EngineWorld;

export namespace helios::engine::runtime::world {


    /**
     * @brief Per-frame context passed to systems during game loop updates.
     *
     * @details UpdateContext bundles frame-scoped data and services used by
     * system updates: timing values, immutable input/viewport snapshots,
     * session/runtime environment access, typed entity access via `EngineWorld`,
     * event-bus read/write channels, and typed ECS access.
     *
     * Command submission is handled by systems through injected command buffers
     * (`CommandBuffer_type`) using `cmdBuffer.template add<TCommand>(...)`.
     *
     * @see GameLoop
     * @see Session
     * @see RuntimeEnvironment
     * @see EngineWorld
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
         * @brief Immutable snapshot of input state for the current frame.
         */
        const helios::engine::input::InputSnapshot& inputSnapshot_;

        /**
         * @brief Reference to the current game session for state tracking.
         */
        helios::engine::runtime::world::Session& session_;

        /**
         * @brief Reference to the current platform entity.
         */
        helios::engine::runtime::world::RuntimeEnvironment& runtimeEnvironment_;

        /**
         * @brief Sink for pushing phase-level events during update.
         *
         * Used by systems and components to publish events (e.g., collision,
         * spawn requests) that will be processed in the next phase of the game loop.
         */
        helios::engine::runtime::messaging::event::GameLoopEventBus::WriteSink phaseEventSink_;

        /**
         * @brief Source for reading phase-level events from the previous phase.
         */
        const helios::engine::runtime::messaging::event::GameLoopEventBus::ReadSource phaseEventSource_;

        /**
         * @brief Sink for pushing pass-level events during update.
         *
         */
        helios::engine::runtime::messaging::event::GameLoopEventBus::WriteSink passEventSink_;

        /**
         * @brief Source for reading pass-level events from previous passes.
         */
        const helios::engine::runtime::messaging::event::GameLoopEventBus::ReadSource passEventSource_;

        /**
         * @brief Sink for pushing frame-level events during update.
         *
         * Used by systems and components to publish events that will be processed
         * in the next frame. Frame-level events persist across all phases and are
         * swapped at the end of the Post phase.
         */
        helios::engine::runtime::messaging::event::GameLoopEventBus::WriteSink frameEventSink_;

        /**
         * @brief Source for reading frame-level events from the previous frame.
         */
        const helios::engine::runtime::messaging::event::GameLoopEventBus::ReadSource frameEventSource_;

        /**
         * @brief Pointer to the active Level, or nullptr if no level is loaded.
         */
        const Level* level_;

        /**
         * @brief Aggregate typed world used for domain-routed ECS operations.
         */
        helios::engine::runtime::world::EngineWorld& engineWorld_;
    public:


        /**
         * @brief Constructs an UpdateContext with all per-frame dependencies.
         *
         * @param session Reference to current session state.
         * @param runtimeEnvironment Reference to runtime-environment state.
         * @param deltaTime Time since last frame in seconds.
         * @param totalTime Accumulated time in seconds.
         * @param phaseEventBus Phase-level event bus.
         * @param passEventBus Pass-level event bus.
         * @param frameEventBus Frame-level event bus.
         * @param inputSnapshot Immutable frame input snapshot.
         * @param viewportSnapshots Immutable frame viewport snapshot set.
         * @param level Active level pointer, or nullptr.
         * @param engineWorld Aggregate typed world for entity operations.
         */
        UpdateContext(
            helios::engine::runtime::world::Session& session,
            helios::engine::runtime::world::RuntimeEnvironment& runtimeEnvironment,
            const float deltaTime,
            const float totalTime,
            helios::engine::runtime::messaging::event::GameLoopEventBus& phaseEventBus,
            helios::engine::runtime::messaging::event::GameLoopEventBus& passEventBus,
            helios::engine::runtime::messaging::event::GameLoopEventBus& frameEventBus,
            const helios::engine::input::InputSnapshot& inputSnapshot,
            const Level* level,
            EngineWorld& engineWorld
        ) :
        session_(session),
        runtimeEnvironment_(runtimeEnvironment),
        deltaTime_(deltaTime),
        totalTime_(totalTime),
        phaseEventSink_(phaseEventBus.writeSink()),
        phaseEventSource_(phaseEventBus.readSource()),
        passEventSink_(passEventBus.writeSink()),
        passEventSource_(passEventBus.readSource()),
        frameEventSink_(frameEventBus.writeSink()),
        frameEventSource_(frameEventBus.readSource()),
        inputSnapshot_(inputSnapshot),
        level_(level),
        engineWorld_(engineWorld)
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
            return engineWorld_.template find<THandle>(handle);
        }

        /**
         * @brief Adds a new entity in the domain inferred from `THandle`.
         *
         * @tparam THandle Handle type.
         *
         * @param strongId Optional strong id value used by the handle domain.
         *
         * @return Domain-specific entity facade for the created entity.
         */
        template<typename THandle>
        [[nodiscard]] auto add(const typename THandle::StrongId_type strongId = typename THandle::StrongId_type{}, const bool isActive = true) noexcept {
            auto entity = engineWorld_.template add<THandle>(strongId);
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
            return engineWorld_.template destroy<THandle>(handle);
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
         * @brief Returns the session for game/match state access.
         *
         * @return Ref to the Session used with this UpdateContext.
         */
        [[nodiscard]] helios::engine::runtime::world::Session& session() const noexcept {
            return session_;
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
         * @brief Pushes an event to the pass-level event bus.
         *
         * @tparam E The event type to push.
         * @tparam Args Constructor argument types for the event.
         *
         * @param args Arguments forwarded to the event constructor.
         *
         * @see readPass()
         * @see Pass::addCommitPoint()
         */
        template<typename E, typename... Args>
        void pushPass(Args&&... args) {
            passEventSink_.template push<E>(std::forward<Args>(args)...);
        }

        /**
         * @brief Pushes an event to the phase-level event bus.
         *
         * @tparam E The event type to push.
         * @tparam Args Constructor argument types for the event.
         *
         * @param args Arguments forwarded to the event constructor.
         *
         * @see readPhase()
         * @see GameLoop
         */
        template<typename E, typename... Args>
        void pushPhase(Args&&... args) {
            phaseEventSink_.template push<E>(std::forward<Args>(args)...);
        }

        /**
         * @brief Reads events from the phase-level event bus.
         *
         * Returns events that were pushed during the previous phase via
         * `pushPhase()`. The phase event bus is swapped at phase boundaries,
         * configured in GameLoop::phaseEnd().
         *
         * @tparam E The event type to read.
         *
         * @return A span of const events of type E.
         *
         * @see pushPhase()
         * @see GameLoop
         */
        template<typename E>
        std::span<const E> readPhase() {
            return phaseEventSource_.template read<E>();
        }

        /**
         * @brief Reads events from the pass-level event bus.
         *
         * @tparam E The event type to read.
         *
         * @return A span of const events of type E.
         *
         * @see pushPass()
         * @see Pass::addCommitPoint()
         */
        template<typename E>
        std::span<const E> readPass() {
            return passEventSource_.template read<E>();
        }

        /**
         * @brief Reads events from the frame-level event bus.
         *
         * @details Returns events that were pushed during the previous frame via
         * `pushFrame()`. The frame event bus is swapped at the end of the Post
         * phase, making events readable in the subsequent frame.
         *
         * Frame-level events are useful for cross-frame communication, such as:
         * - Collision events that trigger effects in the next frame
         * - Spawn confirmations for UI updates
         * - Audio/VFX triggers
         *
         * @tparam E The event type to read.
         *
         * @return A span of const events of type E.
         *
         * @see pushFrame()
         * @see GameLoop
         */
        template<typename E>
        std::span<const E> readFrame() {
            return frameEventSource_.template read<E>();
        }

        /**
         * @brief Pushes an event to the frame-level event bus.
         *
         * @details Events pushed here become readable in the next frame via
         * `readFrame()`. The frame event bus is swapped at the end of the Post
         * phase in GameLoop.
         *
         * Use frame-level events for cross-frame communication where events
         * should persist beyond the current phase.
         *
         * @tparam E The event type to push.
         * @tparam Args Constructor argument types for the event.
         *
         * @param args Arguments forwarded to the event constructor.
         *
         * @see readFrame()
         * @see GameLoop
         */
        template<typename E, typename... Args>
        void pushFrame(Args&&... args) {
            frameEventSink_.template push<E>(std::forward<Args>(args)...);
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
            return engineWorld_.view<THandle, Components...>();
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
            engineWorld_.clearDirtySets<THandle, Components...>();
        }

    };
}

