/**
 * @file RenderTargetEntity.ixx
 * @brief Entity alias for mesh-managed ECS entities.
 */
module;

export module helios.engine.rendering.renderTarget.RenderTargetEntity;

import helios.engine.rendering.renderTarget.types;
import helios.ecs;

using namespace helios::ecs;
export namespace helios::engine::rendering::renderTarget {

    using RenderTargetEntity = Entity<EntityManager<types::RenderTargetHandle>>;


}