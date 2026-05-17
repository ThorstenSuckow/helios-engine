/**
 * @file MaterialHandle.ixx
 * @brief Entity-handle alias for material resources.
 */
module;

export module helios.engine.rendering.material.types.MaterialHandle;

import helios.ecs.types;
import helios.engine.rendering.material.types.MaterialDomainTag;

export namespace helios::engine::rendering::material::types {

    /**
     * @brief Handle type used to reference material entities.
     */
    using MaterialHandle = helios::ecs::types::EntityHandle<MaterialDomainTag>;

}
