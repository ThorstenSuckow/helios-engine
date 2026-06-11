/**
 * @file VertexAttribute.ixx
 * @brief Describes a single logical vertex attribute.
 */
module;

#include <cstdint>

export module helios.engine.rendering.mesh.types.VertexAttribute;

import helios.engine.rendering.mesh.types.VertexAttributeSemantics;
import helios.engine.rendering.mesh.types.VertexAttributeType;


export namespace helios::engine::rendering::mesh::types {


    /**
     * @brief Metadata for one vertex attribute stream entry.
     */
    struct VertexAttribute {

        /**
         * @brief Semantic meaning of the attribute (for example position).
         */
        VertexAttributeSemantics semantics;

        /**
         * @brief Number of scalar components (for example 3 for vec3).
         */
        std::uint32_t size;

        /**
         * @brief Scalar storage type of each component.
         */
        VertexAttributeType type;

    };

}