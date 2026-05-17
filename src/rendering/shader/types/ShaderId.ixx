/**
 * @file ShaderId.ixx
 * @brief Strongly-typed identifier for shader resources.
 */
module;

export module helios.engine.rendering.shader.types.ShaderId;

import helios.engine.rendering.shader.types.ShaderDomainTag;
import helios.ecs.types.StrongId;

export namespace helios::engine::rendering::shader::types {

    /**
     * @brief Strongly-typed identifier for shader resources.
     */
    using ShaderId = helios::ecs::types::StrongId<ShaderDomainTag>;

}
