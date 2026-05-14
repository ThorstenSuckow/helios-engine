/**
 * @file WorldLifecycleAction.ixx
 * @brief Enumeration of world-level lifecycle actions.
 */
module;

#include <cstdint>

export module helios.runtime.lifecycle.types.WorldLifecycleAction;

export namespace helios::runtime::lifecycle::types {

    /**
     * @brief Actions that can be requested via WorldLifecycleCommand.
     */
    enum class WorldLifecycleAction : uint8_t {
        /**
         * @brief Resets all managers and the session to their initial state.
         */
        Reset = 0
    };

}

