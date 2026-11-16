/**
 * @file VertexAttribute.ixx
 * @brief Describes a single logical vertex attribute.
 */
module;


export module helios.engine.rendering.vertexInput.types:VertexAttribute;

import :VertexAttributeSemantics;
import :VertexAttributeType;


export namespace helios::engine::rendering::vertexInput::types {


    /**
     * @brief Metadata for one vertex attribute stream entry.
     */
    struct VertexAttribute {

        /**
         * @brief Semantic meaning of the attribute (for example position).
         */
        VertexAttributeSemantics semantics;

        /**
         * @brief Scalar storage type of each component.
         */
        VertexAttributeType type;

    };

}