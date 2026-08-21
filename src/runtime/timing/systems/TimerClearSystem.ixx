/**
 * @file TimerClearSystem.ixx
 * @brief System that resets finished game timers each frame.
 */
module;

export module helios.engine.runtime.timing.systems.TimerClearSystem;

import helios.engine.runtime.timing.Timer;
import helios.engine.runtime.timing.TimerManager;

import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.types;
import helios.engine.runtime.concepts;

import helios.ecs.system.tags;

import helios.engine.runtime.timing.types;

using namespace helios::engine::runtime::timing;

using namespace helios::engine::runtime::timing::types;

export namespace helios::engine::runtime::timing::systems {

    /**
     * @brief Resets finished game timers to an undefined state.
     *
     * @details Each frame, this system iterates over all registered timers
     * and resets any timer whose state is `TimerState::Finished` back to
     * `TimerState::Undefined`. This ensures that one-shot timer completions
     * are consumed within a single frame and do not trigger downstream
     * logic more than once.
     *
     * @see TimerManager
     * @see Timer
     */
    template<typename TTimerManager>
    class TimerClearSystem {

        /**
         * @brief Reference to the TimerManager owning the timers.
         */
        TTimerManager& timerManager_;

    public:


        using EcsRoleTag = ecs::system::tags::TypedSystemRole;

        /**
         * @brief Constructs the system with a reference to the TimerManager.
         *
         * @param timerManager The manager whose timers are cleared.
         */
        explicit TimerClearSystem(TTimerManager& timerManager)
        : timerManager_(timerManager) {}

        /**
         * @brief Resets all finished timers to TimerState::Undefined.
         *
         * @param updateCtx The current frame's update context.
         */
        template<typename TUpdateContextType>
        requires runtime::concepts::ProvidesUpdateContext<TUpdateContextType, helios::engine::runtime::world::UpdateContext>
        void update(TUpdateContextType& updateCtx) noexcept {

            (void)updateCtx.updateContext();

            for (auto& timer : timerManager_.timers()) {
                if (timer.state() == TimerState::Finished || timer.state() == TimerState::Cancelled) {
                    timer.setState(TimerState::Undefined);
                }
            }
        }

    };

}
