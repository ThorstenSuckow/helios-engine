/**
 * @file EntityPoolCommandBuffer.ixx
 * @brief Concrete command buffer pre-configured with entity pool related commands.
 */
module;

export module helios.engine.runtime.messaging.command.EntityPoolCommandBuffer;


import helios.engine.runtime.messaging.command.TypedCommandBuffer;

import helios.engine.runtime.pooling.commands;
import helios.engine.runtime.world.types;
import helios.engine.runtime.particle.types;


export namespace helios::engine::runtime::messaging::command {

    using EntityPoolCommandBuffer = TypedCommandBuffer<
        pooling::commands::PrefabComponentPoolCommand<world::types::GameObjectHandle>,
        pooling::commands::PrefabComponentPoolCommand<particle::types::ParticleHandle>

    >;
;


}

