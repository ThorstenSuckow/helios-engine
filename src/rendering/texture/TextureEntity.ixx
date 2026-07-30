/**
 * @file TextureEntity.ixx
 * @brief Entity alias for texture-managed ECS entities.
 */
module;

export module helios.engine.rendering.texture.TextureEntity;

import helios.engine.rendering.texture.TextureEntityManager;
import helios.ecs.Entity;

using namespace helios::ecs;
export namespace helios::engine::rendering::texture {

    /** @brief ECS entity alias bound to `TextureEntityManager`. */
    using TextureEntity = Entity<TextureEntityManager>;


}