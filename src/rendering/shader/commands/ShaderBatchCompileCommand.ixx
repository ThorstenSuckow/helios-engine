/**
 * @file ShaderBatchCompileCommand.ixx
 * @brief Command payload for compiling multiple shader resources.
 */
module;

#include <vector>

export module helios.engine.rendering.shader.commands.ShaderBatchCompileCommand;

import helios.engine.rendering.shader.types.ShaderHandle;
import helios.engine.rendering.shader.concepts.IsShaderHandle;

using namespace helios::engine::rendering::shader::concepts;
export namespace helios::engine::rendering::shader::commands {

    /**
     * @brief Requests batched compilation for multiple shader handles.
     *
     * @tparam THandle Shader handle type.
     */
    template<typename THandle>
    requires IsShaderHandle<THandle>
    struct ShaderBatchCompileCommand {
        std::vector<THandle> shaderHandles{};
    };

}