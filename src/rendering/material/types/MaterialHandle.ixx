/**
 * @file MaterialHandle.ixx
 * @brief Entity-handle alias for material resources.
 */
module;

export module helios.engine.rendering.material.types.MaterialHandle;

import helios.ecs.common.types;
import helios.engine.rendering.material.types.MaterialDomainTag;

export namespace helios::engine::rendering::material::types {

    /**
     * @brief Handle type used to reference material entities.
     */
    using MaterialHandle = helios::ecs::common::types::EntityHandle<MaterialDomainTag>;

}
