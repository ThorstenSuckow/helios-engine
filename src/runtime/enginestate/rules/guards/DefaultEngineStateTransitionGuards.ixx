/**
 * @file DefaultEngineStateTransitionGuards.ixx
 * @brief Default guard functions for game state transitions.
 */
module;

#include <cassert>

export module helios.engine.runtime.enginestate.rules.guards.DefaultEngineStateTransitionGuards;

import helios.engine.runtime.gameloop.types;

import helios.engine.runtime.RuntimeEnvironment;

import helios.engine.runtime.enginestate.EngineStateBindings;
import helios.engine.runtime.enginestate.types;
import helios.engine.state.types;

import helios.engine.rendering.shader.components;

using namespace helios::engine::runtime::enginestate::types;
using namespace helios::engine::state::types;
using namespace helios::engine::rendering::shader::components;
export namespace helios::engine::runtime::enginestate::rules::guards {


    class DefaultEngineStateTransitionGuards {

    public:

        /**
         * @brief Guard that checks if a current context component is available.
         *
         * @param runtimeEnvironment
         * @param transitionRequest The requested state transition.
         *
         * @return True if an entity with a CurrentContext exists.
         */
        static bool isPlatformInitialized(
            const RuntimeEnvironment& runtimeEnvironment,
            const StateTransitionRequest<EngineState> transitionRequest
        ) {
            return runtimeEnvironment.isInitialized();
        }

        /**
         * @brief Guard that checks if the runtime infrastructure is available.
         *
         * @param runtimeEnvironment
         * @param transitionRequest The requested state transition.
         *
         * @return True if the infrastructure can be considered in a ready-state.
         */
        static bool isRuntimeInfrastructureReady(
            const RuntimeEnvironment& runtimeEnvironment,
            const StateTransitionRequest<EngineState> transitionRequest
        ) {
            return runtimeEnvironment.isRuntimeInfrastructureReady();
        }


    };


}
