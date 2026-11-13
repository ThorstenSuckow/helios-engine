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

export module helios.engine.rendering.mesh.components.VertexAttributeLayoutsComponent;

import helios.engine.rendering.mesh.types.VertexAttributeSemantics;
import helios.engine.rendering.mesh.types.VertexAttributeLayouts;
import helios.engine.rendering.mesh.types.VertexAttribute;

using namespace helios::engine::rendering::mesh::types;
export namespace helios::engine::rendering::mesh::components {


    /**
     * @brief Stores a semantic-indexed set of vertex attribute layouts.
     *
     * @tparam TOwnerHandle Owning entity handle type.
     * @tparam TVertexInput Marker type describing the input stepping domain.
     */
    struct VertexAttributeLayoutsComponent {

        VertexAttributeLayouts layouts;

    };

}