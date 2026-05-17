/**
 * @file EngineStateBindings.ixx
 * @brief Engine-internal binding: maps EngineState to EngineStateTransitionId.
 *
 * @details This specialization belongs to the engine itself so that engine
 * modules (Session, EngineFlowSystem, guards, rules, ...) can resolve
 * `StateTransitionId<EngineState>::Type`.
 */
module;

export module helios.engine.runtime.enginestate.EngineStateBindings;

export import helios.engine.state.types.StateTransitionId;
export import helios.engine.runtime.enginestate.types.EngineState;
export import helios.engine.runtime.enginestate.types.EngineStateTransitionId;

export namespace helios::engine::state::types {

    /**
     * @brief Binds EngineState to EngineStateTransitionId.
     *
     * @see StateTransitionId
     * @see helios::engine::runtime::enginestate::types::EngineState
     * @see helios::engine::runtime::enginestate::types::EngineStateTransitionId
     */
    template<>
    struct StateTransitionId<helios::engine::runtime::enginestate::types::EngineState> {
        using Type = helios::engine::runtime::enginestate::types::EngineStateTransitionId;
    };

}

