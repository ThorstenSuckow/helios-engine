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
import helios.engine.rendering.shader.types;
import helios.engine.runtime.world.types;


using namespace helios::engine::rendering::shader::types;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::world::types;
export namespace helios::engine::runtime::messaging::command {

    using RenderCommandBuffer  = helios::engine::runtime::messaging::command::TypedCommandBuffer<
        helios::engine::rendering::common::commands::RenderCommand<GameObjectHandle>,
        helios::engine::rendering::shader::commands::ShaderCompileCommand<ShaderHandle>,
        helios::engine::rendering::shader::commands::ShaderBatchCompileCommand<ShaderHandle>
    >;
;


}

