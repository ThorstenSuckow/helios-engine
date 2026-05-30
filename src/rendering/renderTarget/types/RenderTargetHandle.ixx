/**
 * @file RenderTargetHandle.ixx
 * @brief Entity-handle alias for renderTarget resources.
 */
module;

export module helios.engine.rendering.renderTarget.types.RenderTargetHandle;


import helios.ecs.types;
import helios.engine.rendering.renderTarget.types.RenderTargetDomainTag;

export namespace helios::engine::rendering::renderTarget::types {

    /**
     * @brief Handle type used to reference renderTarget entities.
     */
    using RenderTargetHandle = helios::ecs::types::EntityHandle<RenderTargetDomainTag>;

}
