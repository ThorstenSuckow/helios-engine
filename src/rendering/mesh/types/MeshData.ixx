/**
 * @file MeshData.ixx
 * @brief Plain mesh payload containing vertex/index buffers and primitive topology.
 */
module;

#include <span>
#include <vector>

export module helios.engine.rendering.mesh.types.MeshData;

import helios.engine.rendering.common.types.Vertex;
import helios.engine.rendering.mesh.types.PrimitiveType;

using namespace helios::engine::rendering::common::types;;

export namespace helios::engine::rendering::mesh::types {

    /**
     * @brief Raw mesh data container used by mesh registries and render backends.
     */
    struct MeshData {

        /**
         * @brief Vertex buffer payload.
         */
        std::vector<Vertex> vertices;

        /**
         * @brief Index buffer payload.
         */
        std::vector<unsigned int> indices;

        /**
         * @brief Primitive topology used to interpret the index buffer.
         */
        PrimitiveType primitiveType;

        /**
         * @brief Constructs mesh data from owned vertex/index vectors.
         * @param vertices Vertex buffer payload.
         * @param indices Index buffer payload.
         * @param primitiveType Primitive topology for draw interpretation.
         */
        explicit MeshData(std::vector<Vertex> vertices, std::vector<unsigned int> indices, PrimitiveType primitiveType)
        : vertices(std::move(vertices)), indices(std::move(indices)), primitiveType(primitiveType) {};

        /**
         * @brief Constructs mesh data by copying from non-owning spans.
         * @param vertices Vertex buffer view.
         * @param indices Index buffer view.
         * @param primitiveType Primitive topology for draw interpretation.
         */
        explicit MeshData(std::span<const Vertex> vertices, std::span<const unsigned int> indices, PrimitiveType primitiveType)
        : vertices(vertices.begin(), vertices.end()), indices(indices.begin(), indices.end()), primitiveType(primitiveType) {};


    };
}