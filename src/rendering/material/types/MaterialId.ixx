/**
 * @file MaterialId.ixx
 * @brief Strongly-typed identifier for material resources.
 */
module;

export module helios.engine.rendering.material.types.MaterialId;

import helios.core.common.types;
import helios.engine.rendering.material.types.MaterialDomainTag;

export namespace helios::engine::rendering::material::types {

    /**
     * @brief Strongly-typed identifier for material resources.
     */
    using MaterialId = helios::core::common::types::StrongId<MaterialDomainTag>;

}
