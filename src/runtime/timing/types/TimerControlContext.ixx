/**
 * @file TimerControlContext.ixx
 * @brief Context information for state transitions.
 */
module;


export module helios.engine.runtime.timing.types.TimerControlContext;

import helios.engine.runtime.timing.types.TimerState;
import helios.engine.runtime.timing.types.TimerId;

using namespace helios::engine::runtime::timing::types;

export namespace helios::engine::runtime::timing::types {

    /**
     * @brief Context describing a requested timer state transition.
     *
     * Carries the target TimerState and the TimerId of the timer
     * to be controlled.
     *
     * @see TimerControlCommand
     * @see TimerManager
     */
    struct TimerControlContext {

        /**
         * @brief The id of the timer to control.
        */
        const TimerId timerId;

        /**
         * @brief The target state for the timer.
         */
        const TimerState timerState;

        /**
         * @brief Indicates whether the timer's elapsed seconds should be reset to 0 before applying
         * the new state.
         */
        const bool resetElapsed = false;
    };

}