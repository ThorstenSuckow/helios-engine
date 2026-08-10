/**
 * @file ViewportEntity.ixx
 * @brief Entity alias for viewport-managed ECS entities.
 */
module;

export module helios.engine.rendering.viewport.ViewportEntity;

import helios.engine.rendering.viewport.types;
import helios.ecs;

using namespace helios::ecs;
export namespace helios::engine::rendering::viewport {

    using ViewportEntity = Entity<EntityManager<types::ViewportHandle>>;


}