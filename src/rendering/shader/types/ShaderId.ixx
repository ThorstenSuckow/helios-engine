/**
 * @file ShaderId.ixx
 * @brief Strongly-typed identifier for shader resources.
 */
module;

export module helios.engine.rendering.shader.types.ShaderId;

import helios.engine.rendering.shader.types.ShaderDomainTag;
import helios.core.common.types;

export namespace helios::engine::rendering::shader::types {

    /**
     * @brief Strongly-typed identifier for shader resources.
     */
    using ShaderId = helios::core::common::types::StrongId<ShaderDomainTag>;

}
