/**
 * @file EngineFlowSystem.ixx
 * @brief System that drives the high-level game state flow.
 */
module;

export module helios.engine.runtime.enginestate.systems.EngineFlowSystem;

import helios.engine.runtime.enginestate.types;

import helios.engine.runtime.messaging.command.concepts;

import helios.engine.state.commands;
import helios.engine.state.types.StateTransitionRequest;
import helios.engine.state.types.StateTransitionId;
import helios.engine.runtime.enginestate.EngineStateBindings;

import helios.engine.runtime.enginestate.types;

import helios.ecs;
import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.Session;

import helios.engine.runtime.world.tags.SystemRole;

using namespace helios::engine::state::commands;
using namespace helios::engine::runtime::messaging::command::concepts;
using namespace helios::engine::state::types;
using namespace helios::engine::runtime::enginestate;
using namespace helios::engine::runtime::enginestate::types;
export namespace helios::engine::runtime::enginestate::systems {

    /**
     * @brief System that drives automatic game state transitions.
     *
     * EngineFlowSystem monitors the current EngineState and issues StateCommands
     * to advance the state machine when appropriate. It tracks the previous
     * state and transition ID to avoid emitting duplicate commands.
     *
     * Typical flow: Undefined -> Start -> Title.
     *
     * @see EngineState
     * @see EngineStateTransitionId
     * @see StateCommand
     */
    template<typename TCommandBuffer>
    requires IsCommandBufferLike<TCommandBuffer>
    class EngineFlowSystem {

        /**
         * @brief The previously observed game state.
         */
        EngineState prevMatchState_ = EngineState::Undefined;

        /**
         * @brief The previously observed state transition ID.
         */
        StateTransitionIdType<EngineState> prevEngineStateTransitionId_ = StateTransitionIdType<EngineState>::Undefined;

    public:

        using EngineRoleTag = helios::engine::runtime::world::tags::SystemRole;

        using CommandBuffer_type = TCommandBuffer;

        /**
         * @brief Updates the game flow and emits state transition commands.
         *
         * Checks the current EngineState from the session and compares it with
         * the previously observed state. If a transition is required, a
         * StateCommand is added to the command buffer.
         *
         * @param updateContext The update context providing session and command buffer access.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext, TCommandBuffer& buffer) noexcept {

            auto& session = updateContext.session();

            const auto engineState = session.state<EngineState>();
            auto engineStateTransitionId = session.stateTransitionId<EngineState>();

            // only return if the state is undefined.
            // in any other case, we allow to drive the state forward, even if
            // the previous state is the same as the current one, in case any guard
            // did veto the next state
            if (engineState == EngineState::Undefined) {// && prevMatchState_ == engineState && prevEngineStateTransitionId_ == engineStateTransitionId) {
                return;
            }

            prevMatchState_= engineState;
            prevEngineStateTransitionId_ = engineStateTransitionId;

            switch (engineState) {
                case EngineState::Booting: {
                    buffer.template add<StateCommand<EngineState>>(
                        StateTransitionRequest<EngineState>(engineState, EngineStateTransitionId::BootRequest)
                    );
                    break;
                }

                case EngineState::Booted: {
                    buffer.template add<StateCommand<EngineState>>(
                        StateTransitionRequest<EngineState>(engineState, EngineStateTransitionId::WarmupRequest)
                    );
                    break;
                }


                default:
                    break;
            }
        }
    };
}
