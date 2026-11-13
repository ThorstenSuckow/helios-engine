/**
 * @file VertextAttributeMappingsComponent.ixx
 * @brief Component that stores semantic-to-vertex-attribute-name mappings for shader usage.
 */
module;

export module helios.engine.rendering.shader.components.VertexAttributeMappingsComponent;

import helios.engine.rendering.shader.types.VertexAttributeMappings;

export namespace helios::engine::rendering::shader::components {

    /**
     * @brief Stores vertext attribute names indexed by `VertexAttributeSemantics`.
     */
    struct VertexAttributeMappingsComponent {

        types::VertexAttributeMappings mappings;

    };

}