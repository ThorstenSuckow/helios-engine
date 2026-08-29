/**
 * @file ShaderBatchCompileCommand.ixx
 * @brief Command payload for compiling multiple shader resources.
 */
module;

#include <vector>

export module helios.engine.rendering.shader.commands.ShaderBatchCompileCommand;

export namespace helios::engine::rendering::shader::commands {

    /**
     * @brief Requests batched compilation for multiple shader handles.
     *
     * @tparam THandle Shader handle type.
     */
    template<typename THandle>
    struct ShaderBatchCompileCommand {
        std::vector<THandle> shaderHandles{};
    };

}