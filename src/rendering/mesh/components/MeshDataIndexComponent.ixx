/**
 * @file MeshDataComponent.ixx
 * @brief ECS component that references mesh data in the mesh resource storage.
 */
module;

export module helios.engine.rendering.mesh.components.MeshDataIndexComponent;

import helios.engine.rendering.mesh.types.MeshDataIndex;

using namespace helios::engine::rendering::mesh::types;
export namespace helios::engine::rendering::mesh::components {

    /**
     * @brief Component that links an entity to a mesh-data entry.
     */
    template<typename TWOwnerHandle>
    struct MeshDataIndexComponent {

        /**
         * @brief Index into mesh data storage used by rendering systems.
         */
        MeshDataIndex meshDataIndex;

    };


}