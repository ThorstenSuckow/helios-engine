/**
 * @file MeshData.ixx
 * @brief Plain mesh payload containing vertices and indices.
 */
module;

#include <vector>

export module helios.engine.rendering.mesh.types.MeshData;

import helios.engine.rendering.Vertex;

using namespace helios::engine::rendering;

export namespace helios::engine::rendering::mesh::types {

    /**
     * @brief Raw mesh data container.
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

    };
}