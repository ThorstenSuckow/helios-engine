/**
 * @file ShaderEntity.ixx
 * @brief ECS facade alias for shader resource entities.
 */
module;

export module helios.engine.rendering.shader.ShaderEntity;

import helios.engine.rendering.shader.ShaderEntityManager;
import helios.ecs.Entity;

using namespace helios::ecs;
export namespace helios::engine::rendering::shader {

    /**
     * @brief Entity facade bound to `ShaderEntityManager`.
     */
    using ShaderEntity = Entity<ShaderEntityManager>;

} // namespace helios::engine::rendering::shader
