/**
 * @file ViewportHandle.ixx
 * @brief Entity-handle alias for viewport resources.
 */
module;

export module helios.engine.rendering.viewport.types.ViewportHandle;


import helios.ecs.common.types;
import helios.engine.rendering.viewport.types.ViewportDomainTag;

export namespace helios::engine::rendering::viewport::types {

    /**
     * @brief Handle type used to reference viewport entities.
     */
    using ViewportHandle = helios::ecs::common::types::EntityHandle<ViewportDomainTag>;

}
