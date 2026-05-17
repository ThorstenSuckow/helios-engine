/**
 * @file MaterialEntity.ixx
 * @brief Entity alias for material-managed ECS entities.
 */
module;

export module helios.engine.rendering.material.MaterialEntity;

import helios.engine.rendering.material.MaterialEntityManager;
import helios.ecs.Entity;

using namespace helios::ecs;
export namespace helios::engine::rendering::material {

    /** @brief ECS entity alias bound to `MaterialEntityManager`. */
    using MaterialEntity = Entity<MaterialEntityManager>;


}