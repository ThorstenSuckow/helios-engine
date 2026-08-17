/**
 * @file MeshHandle.ixx
 * @brief Entity-handle alias for mesh resources.
 */
module;

export module helios.engine.rendering.mesh.types.MeshHandle;


import helios.ecs.common.types;
import helios.engine.rendering.mesh.types.MeshDomainTag;

export namespace helios::engine::rendering::mesh::types {

    /**
     * @brief Handle type used to reference mesh entities.
     */
    using MeshHandle = helios::ecs::common::types::EntityHandle<MeshDomainTag>;

}
