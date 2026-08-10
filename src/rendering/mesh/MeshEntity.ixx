/**
 * @file MeshEntity.ixx
 * @brief Entity alias for mesh-managed ECS entities.
 */
module;

export module helios.engine.rendering.mesh.MeshEntity;

import helios.engine.rendering.mesh.types;
import helios.ecs;

using namespace helios::ecs;
export namespace helios::engine::rendering::mesh {

    using MeshEntity = Entity<EntityManager<types::MeshHandle>>;


}