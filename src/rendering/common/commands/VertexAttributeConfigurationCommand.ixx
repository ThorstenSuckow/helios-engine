module;


export module helios.engine.rendering.common.commands:VertexAttributeConfigurationCommand;

import helios.engine.rendering.mesh.types.VertexAttributeLayouts;
import helios.engine.rendering.shader.types.VertexAttributeMappings;

export namespace helios::engine::rendering::common::commands {

    /**
     * @brief Command to update the configuration of vertex attributes for a mesh and shader combination.
     *
     * @tparam TRenderHandles Render handles type.
     */
    template<typename TRenderHandles>
    struct VertexAttributeConfigurationCommand {

        /**
         * @brief Handle to the mesh for which vertex attributes are being updated.
         */
        typename TRenderHandles::MeshHandle meshHandle;

        shader::types::VertexAttributeMappings mappings;

        mesh::types::VertexAttributeLayouts layouts;;

    };

}