/**
 * @file VertexAttributeLayoutComponent.ixx
 * @brief Component that stores active vertex attribute layouts by semantic.
 */
module;

#include <string>
#include <array>
#include <bitset>
#include <cstddef>
#include <utility>
#include <cassert>
#include "helios-engine-config.h"

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

    using InstanceDataLayoutComponent = VertexBufferLayoutComponent;
}