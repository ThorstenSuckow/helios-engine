/**
 * @file EngineCommandBuffer.ixx
 * @brief Concrete command buffer pre-configured with all engine command types.
 */
module;

export module helios.engine.runtime.messaging.command.EngineCommandBuffer;


import helios.engine.platform.environment.types;
import helios.engine.platform.environment.commands;
import helios.engine.platform.window.commands;


import helios.engine.platform.lifecycle.commands;

import helios.engine.state;
import helios.engine.runtime.enginestate;


import helios.engine.runtime.messaging.command.TypedCommandBuffer;


import helios.engine.runtime.timing.commands;

import helios.engine.runtime.lifecycle.commands.WorldLifecycleCommand;

import helios.engine.runtime.world.types;
import helios.engine.runtime.messaging.command.tags.CommandBufferRole;

using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::world::types;
export namespace helios::engine::runtime::messaging::command {

    using EngineCommandBuffer = helios::engine::runtime::messaging::command::TypedCommandBuffer<

            helios::engine::runtime::timing::commands::TimerControlCommand,
            helios::engine::runtime::lifecycle::commands::WorldLifecycleCommand,

            helios::engine::state::commands::StateCommand<helios::engine::runtime::enginestate::types::EngineState>,
            helios::engine::state::commands::DelayedStateCommand<helios::engine::runtime::enginestate::types::EngineState>


        >;

}

