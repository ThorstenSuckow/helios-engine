/**
 * @file TextureEntity.ixx
 * @brief Entity alias for texture-managed ECS entities.
 */
module;

export module helios.engine.rendering.texture.TextureEntity;

import helios.engine.rendering.texture.types;
import helios.ecs;

using namespace helios::ecs;
export namespace helios::engine::rendering::texture {


    using TextureEntity = Entity<EntityManager<types::TextureHandle>>;


}