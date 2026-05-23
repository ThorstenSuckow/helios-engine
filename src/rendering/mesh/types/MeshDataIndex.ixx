/**
 * @file MeshDataIndex.ixx
 * @brief Index type used to address entries in mesh-data storage.
 */
module;

#include <cstddef>

export module helios.engine.rendering.mesh.types.MeshDataIndex;


export namespace helios::engine::rendering::mesh::types {

    /**
     * @brief Zero-based index into mesh data containers.
     */
    using MeshDataIndex = std::size_t;

}
