/**
 * @file RenderTargetEntity.ixx
 * @brief Entity alias for mesh-managed ECS entities.
 */
module;

export module helios.engine.rendering.renderTarget.RenderTargetEntity;

import helios.engine.rendering.renderTarget.RenderTargetEntityManager;
import helios.ecs.Entity;

using namespace helios::ecs;
export namespace helios::engine::rendering::renderTarget {

    /** @brief ECS entity alias bound to `RenderTargetEntityManager`. */
    using RenderTargetEntity = Entity<RenderTargetEntityManager>;


}