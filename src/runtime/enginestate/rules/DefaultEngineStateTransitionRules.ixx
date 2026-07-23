/**
 * @file DefaultEngineStateTransitionRules.ixx
 * @brief Default transition rules for the game state machine.
 */
module;

#include <span>
#include <array>
#include <cstddef>

export module helios.engine.runtime.enginestate.rules.DefaultEngineStateTransitionRules;



import helios.engine.runtime.world.UpdateContext;

import helios.engine.runtime.enginestate.types;
import helios.engine.runtime.enginestate.rules.guards;

import helios.engine.runtime.enginestate.EngineStateBindings;
import helios.engine.state.types.StateTransitionRule;
import helios.engine.state.types.StateTransitionId;
import helios.engine.rendering.shader.types.ShaderHandle;

using namespace helios::engine::state::types;
using namespace helios::engine::rendering::shader::types;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::enginestate::rules::guards;
export namespace helios::engine::runtime::enginestate::rules {

    using namespace helios::engine::runtime::enginestate::types;
    using namespace helios::engine::state::types;

    /**
     * @brief Provides the default set of game state transition rules.
     */
    class DefaultEngineStateTransitionRules {


        static constexpr auto rules_ = [] {
            std::array baseRules = std::to_array<StateTransitionRule<EngineState>>({

            {EngineState::Undefined,  EngineStateTransitionId::BootRequest,       EngineState::Booting},
            {EngineState::Booting,    EngineStateTransitionId::BootRequest,       EngineState::Booted,
                &DefaultEngineStateTransitionGuards::isPlatformInitialized},

            {EngineState::Booted,     EngineStateTransitionId::WarmupRequest,     EngineState::Warmup,
                &DefaultEngineStateTransitionGuards::isRuntimeInfrastructureReady},

            {EngineState::Warmup,      EngineStateTransitionId::WarmupDone, EngineState::Running},

            });

            constexpr EngineState shutdownable[] = {
                EngineState::Running,
                EngineState::Warmup
            };

            std::array<StateTransitionRule<EngineState>, baseRules.size() + std::size(shutdownable)> rules{};

            for (size_t i = 0; i < baseRules.size(); i++) {
                rules[i] = baseRules[i];
            }

            for (size_t i = 0; i < std::size(shutdownable); i++) {
                rules[baseRules.size() + i] = StateTransitionRule<EngineState>{
                    shutdownable[i],
                    EngineStateTransitionId::ShutdownRequest,
                    EngineState::Shutdown
                };
            }

            return rules;
        }();



    public:

        /**
         * @brief Returns the default transition rules.
         *
         * @return A span of the predefined transition rules.
         */
        [[nodiscard]] static std::span<const StateTransitionRule<EngineState>> rules() {
            return std::span{rules_};
        }

    };
    }

