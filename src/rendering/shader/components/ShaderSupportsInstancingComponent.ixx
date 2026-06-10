/**
 * @file ShaderSupportsInstancingComponent.ixx
 * @brief Marker component that indicates shader support for GPU instancing.
 */
module;

export module helios.engine.rendering.shader.components.ShaderSupportsInstancingComponent;

export namespace helios::engine::rendering::shader::components {

    /**
     * @brief Marker attached to shader entities that can be used for instanced rendering.
     *
     * @tparam TOwnerHandle Owning entity handle type.
     */
    template<typename TOwnerHandle>
    struct ShaderSupportsInstancingComponent {};


}