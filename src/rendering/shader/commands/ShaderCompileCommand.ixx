/**
 * @file ShaderCompileCommand.ixx
 * @brief Command payload for compiling a single shader resource.
 */
module;

export module helios.engine.rendering.shader.commands.ShaderCompileCommand;

export namespace helios::engine::rendering::shader::commands {

    /**
     * @brief Requests compilation of one shader handle.
     *
     * @tparam THandle Shader handle type.
     */
    template<typename THandle>
    struct ShaderCompileCommand {
        THandle shaderHandle;
    };

}