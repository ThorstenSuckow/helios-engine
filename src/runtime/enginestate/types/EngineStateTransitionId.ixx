/**
 * @file EngineStateTransitionId.ixx
 * @brief Identifiers for game state transitions.
 */
module;

#include <cstdint>

export module helios.engine.runtime.enginestate.types.EngineStateTransitionId;
import helios.engine.runtime.enginestate.types.EngineState;



export namespace helios::engine::runtime::enginestate::types {

    /**
     * @brief Enumeration of transition identifiers.
     *
     * Each identifier represents a specific type of state transition request.
     */
    enum class  EngineStateTransitionId : uint16_t  {

        Undefined = 0,

        BootRequest,

        WarmupRequest,

        WarmupDone,

        ShutdownRequest

    };

}

