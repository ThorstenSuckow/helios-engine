/**
 * @file RenderTargetId.ixx
 * @brief Strongly-typed identifier for renderTarget resources.
 */
module;

export module helios.engine.rendering.renderTarget.types.RenderTargetId;

import helios.ecs.types.StrongId;
import helios.engine.rendering.renderTarget.types.RenderTargetDomainTag;

export namespace helios::engine::rendering::renderTarget::types {

    /**
     * @brief Strongly-typed identifier for renderTarget resources.
     */
    using RenderTargetId = helios::ecs::types::StrongId<RenderTargetDomainTag>;

}
