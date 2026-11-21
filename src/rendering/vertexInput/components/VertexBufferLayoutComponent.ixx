/**
 * @file VertexBufferLayoutComponent.ixx
 * @brief Component that stores active vertex buffer layouts.
 */
module;

export module helios.engine.rendering.vertexInput.components:VertexBufferLayoutComponent;

import helios.engine.rendering.vertexInput.types;

export namespace helios::engine::rendering::vertexInput::components {


    /**
     * @brief Stores a semantic-indexed set of vertex attribute layouts.
     *
     * @tparam TOwnerHandle Owning entity handle type.
     * @tparam TVertexInput Marker type describing the input stepping domain.
     */
    struct VertexBufferLayoutComponent {
        types::VertexBufferLayout vertexBufferLayout;
    };


}