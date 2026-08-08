/**
 * @file MeshEntityManager.ixx
 * @brief Mesh-domain ECS registry and manager aliases.
 */
module;

export module helios.engine.rendering.mesh.MeshEntityManager;

import helios.ecs.EntityManager;
import helios.ecs.EntityRegistry;
import helios.ecs.strategies;

import helios.engine.rendering.mesh.types;

using namespace helios::engine::rendering::mesh::types;
using namespace helios::ecs;
using namespace helios::ecs::strategies;
export namespace helios::engine::rendering::mesh {

    /**
     * @brief Entity registry used for mesh resources.
     */
    using MeshEntityRegistry = EntityRegistry<MeshDomainTag, HashedLookupStrategy<>, true>;

    /**
     * @brief Entity manager used for mesh handles and mesh components.
     */
    using MeshEntityManager = EntityManager<MeshHandle, MeshEntityRegistry>;


}