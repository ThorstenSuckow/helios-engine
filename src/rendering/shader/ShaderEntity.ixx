/**
 * @file ShaderEntity.ixx
 * @brief ECS facade alias for shader resource entities.
 */
module;

export module helios.engine.rendering.shader.ShaderEntity;

import helios.engine.rendering.shader.types;
import helios.ecs;

using namespace helios::ecs;
export namespace helios::engine::rendering::shader {


    using ShaderEntity = Entity<EntityManager<types::ShaderHandle>>;

} // namespace helios::engine::rendering::shader
