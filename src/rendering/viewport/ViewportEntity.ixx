/**
 * @file ViewportEntity.ixx
 * @brief Entity alias for viewport-managed ECS entities.
 */
module;

export module helios.engine.rendering.viewport.ViewportEntity;

import helios.engine.rendering.viewport.ViewportEntityManager;
import helios.ecs.Entity;

using namespace helios::ecs;
export namespace helios::engine::rendering::viewport {

    /** @brief ECS entity alias bound to `ViewportEntityManager`. */
    using ViewportEntity = Entity<ViewportEntityManager>;


}