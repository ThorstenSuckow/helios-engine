/**
 * @file VertexAttributeLayout.ixx
 * @brief Declares GPU input layout information for one vertex attribute.
 */
module;

#include <cstdint>

export module helios.engine.rendering.mesh.types.VertexAttributeLayout;

import helios.engine.rendering.mesh.types.VertexAttribute;

export namespace helios::engine::rendering::mesh::types {

    /**
     * @brief Full binding description for one attribute in a vertex layout.
     */
    struct VertexAttributeLayout {

        /**
         * @brief Logical attribute description.
         */
        VertexAttribute attribute;

        /**
         * @brief Shader input location index.
         */
        std::uint32_t location;

        /**
         * @brief Byte stride between consecutive elements.
         */
        std::uint32_t stride;

        /**
         * @brief Byte offset to this attribute within one element.
         */
        std::uint32_t offset;

        /**
         * @brief Instance divisor (`0` per-vertex, `>0` instanced step rate).
         */
        std::uint32_t divisor;



    };


}