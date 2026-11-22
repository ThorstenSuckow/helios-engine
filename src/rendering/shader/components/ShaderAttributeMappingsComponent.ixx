/**
* @file ShaderAttributeMappingsComponent.ixx
 * @brief Component that stores semantic-to-vertex-attribute-name mappings for shader usage.
 */
module;

export module helios.engine.rendering.shader.components:ShaderAttributeMappingsComponent;

import helios.engine.rendering.shader.types;

export namespace helios::engine::rendering::shader::components {

    /**
     * @brief Stores shader attribute names indexed by `ShaderAttributeSemantics`.
     */
    struct ShaderAttributeMappingsComponent {

        types::ShaderAttributeMappings shaderAttributeMappings;

    };

}