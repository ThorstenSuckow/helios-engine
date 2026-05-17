/**
 * @file MeshEntityManager.ixx
 * @brief Mesh-domain ECS registry and manager aliases.
 */
module;

#include "helios-engine-config.h"

export module helios.engine.rendering.mesh.MeshEntityManager;

import helios.ecs.EntityManager;
import helios.ecs.EntityRegistry;
import helios.ecs.strategies;

import helios.engine.rendering.mesh.types;

constexpr auto MESH_SIZE = DEFAULT_MESH_POOL_CAPACITY;

using namespace helios::engine::rendering::mesh::types;
using namespace helios::ecs;
using namespace helios::ecs::strategies;
export namespace helios::engine::rendering::mesh {

    /**
     * @brief Entity registry used for mesh resources.
     */
    using MeshEntityRegistry = EntityRegistry<MeshDomainTag, HashedLookupStrategy, true, MESH_SIZE>;

    /**
     * @brief Entity manager used for mesh handles and mesh components.
     */
    using MeshEntityManager = EntityManager<MeshHandle, MeshEntityRegistry, MESH_SIZE>;


}