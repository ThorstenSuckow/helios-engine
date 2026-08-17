/**
* @file TimerId.ixx
 * @brief Strongly-typed identifier for timers.
 */
module;

#include <cstdint>
#include <string_view>

export module helios.engine.runtime.timing.types.TimerId;

import helios.core.common.types;

export namespace helios::engine::runtime::timing::types {

    /**
     * @brief Tag type for TimerId.
     */
    struct TimerIdTag{};

    /**
     * @brief Strongly-typed identifier for timers.
     *
     * @details Used to uniquely identify timers.
     *
     * @see helios::core::common::types::StrongId
     */
    using TimerId = helios::core::common::types::StrongId<TimerIdTag>;

}
