/**
 * @file VertexAttributeLayout.ixx
 * @brief Declares GPU input layout information for one vertex attribute.
 */
module;

#include <cstdint>


export module helios.engine.rendering.vertexInput.types:VertexAttributeLayout;

import :VertexAttribute;

export namespace helios::engine::rendering::vertexInput::types {

    /**
     * @brief Full binding description for one attribute in a vertex layout.
     */
    struct VertexAttributeLayout {

        /**
         * @brief Logical attribute description.
         */
        VertexAttribute attribute;

        /**
         * @brief Byte offset to this attribute within one element.
         */
        std::uint32_t offset;

    };


}