/**
 * @file RenderCommandBuffer.ixx
 * @brief Concrete command buffer pre-configured with render related engine types.
 */
module;

#include <utility>
#include <type_traits>

export module helios.engine.runtime.messaging.command.RenderCommandBuffer;


import helios.engine.runtime.messaging.command.TypedCommandBuffer;

import helios.engine.rendering.common.commands;
import helios.engine.rendering.shader.commands;
import helios.engine.rendering.mesh.commands;
import helios.engine.rendering.shader.types;
import helios.engine.rendering.texture.types;
import helios.engine.rendering.texture.commands;
import helios.engine.rendering.mesh.types;
import helios.engine.runtime.particle.types;
import helios.engine.runtime.world.types;

export namespace helios::engine::runtime::messaging::command {

    using RenderCommandBuffer  = runtime::messaging::command::TypedCommandBuffer<
        rendering::common::commands::RenderSceneCommand<world::types::GameObjectHandle>,
        rendering::common::commands::RenderSceneMemberCommand<world::types::GameObjectHandle>,
        rendering::common::commands::RenderInstanceBatchCommand<world::types::GameObjectHandle>,
        rendering::common::commands::RenderSceneCommand<particle::types::ParticleHandle>,
        rendering::common::commands::RenderSceneMemberCommand<particle::types::ParticleHandle>,
        rendering::common::commands::RenderInstanceBatchCommand<particle::types::ParticleHandle>,
        rendering::shader::commands::ShaderCompileCommand<rendering::shader::types::ShaderHandle>,
        rendering::shader::commands::ShaderBatchCompileCommand<rendering::shader::types::ShaderHandle>,
        rendering::texture::commands::TextureBatchUploadCommand<rendering::texture::types::TextureHandle>,
        rendering::mesh::commands::MeshBatchUploadCommand<rendering::mesh::types::MeshHandle>

    >;
;


}

