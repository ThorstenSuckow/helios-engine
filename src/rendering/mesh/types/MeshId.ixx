/**
 * @file MeshId.ixx
 * @brief Strongly-typed identifier for mesh resources.
 */
module;

export module helios.engine.rendering.mesh.types.MeshId;

import helios.ecs.types.StrongId;
import helios.engine.rendering.mesh.types.MeshDomainTag;

export namespace helios::engine::rendering::mesh::types {

    /**
     * @brief Strongly-typed identifier for mesh resources.
     */
    using MeshId = helios::ecs::types::StrongId<MeshDomainTag>;

}
