module;


export module helios.engine.rendering.vertexInput.commands:VertexInputSetupCommand;

import helios.engine.rendering.vertexInput.types;
import helios.engine.rendering.shader.types;

export namespace helios::engine::rendering::vertexInput::commands {

    /**
     * @brief Command to update the configuration of vertex attributes for a mesh and shader combination.
     *
     * @tparam TRenderHandles Render handles type.
     */
    template<typename TVertexInputHandle, typename TRenderHandles>
    struct VertexInputSetupCommand {

        TVertexInputHandle vertexInputHandle;

        typename TRenderHandles::MeshHandle meshHandle;

        typename TRenderHandles::ShaderHandle shaderHandle;

        shader::types::VertexAttributeMappings mappings;

        vertexInput::types::VertexBufferLayout vertexBufferLayout;

        vertexInput::types::VertexBufferLayout instanceDataBufferLayout;

    };

}