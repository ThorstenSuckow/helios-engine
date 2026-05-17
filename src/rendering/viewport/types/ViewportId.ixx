/**
 * @file ViewportId.ixx
 * @brief Strongly-typed identifier for viewport resources.
 */
module;

export module helios.engine.rendering.viewport.types.ViewportId;

import helios.ecs.types.StrongId;
import helios.engine.rendering.viewport.types.ViewportDomainTag;

export namespace helios::engine::rendering::viewport::types {

    /**
     * @brief Strongly-typed identifier for viewport resources.
     */
    using ViewportId = helios::ecs::types::StrongId<ViewportDomainTag>;

}
