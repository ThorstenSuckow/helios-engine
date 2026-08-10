/**
 * @file MaterialEntity.ixx
 * @brief Entity alias for material-managed ECS entities.
 */
module;

export module helios.engine.rendering.material.MaterialEntity;

import helios.engine.rendering.material.types;
import helios.ecs;

using namespace helios::ecs;
export namespace helios::engine::rendering::material {

    using MaterialEntity = Entity<EntityManager<types::MaterialHandle>>;


}