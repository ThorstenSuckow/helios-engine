/**
 * @file EngineStateManager.ixx
 * @brief Type alias for game state manager.
 */
module;

#include <memory>
#include <span>
#include <vector>

export module helios.engine.runtime.enginestate.EngineStateManager;

import helios.engine.state;
import helios.engine.runtime.enginestate.types;

export namespace helios::engine::runtime::enginestate {

    /**
     * @brief Type alias for the game state manager.
     *
     * @details Instantiation of the generic StateManager template for
     * EngineState. Manages application-level states (Title, Running, Paused).
     *
     * @see helios::engine::state::StateManager
     * @see types::EngineState
     */
    using EngineStateManager = helios::engine::state::StateManager<types::EngineState>;
}
