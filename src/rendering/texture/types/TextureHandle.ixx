/**
* @file TextureHandle.ixx
 * @brief Entity-handle alias for viewport resources.
 */
module;

export module helios.engine.rendering.texture.types:TextureHandle;

import helios.ecs.types;

export namespace helios::engine::rendering::texture::types {
    
    struct TextureDomainTag{};

    using TextureId = helios::ecs::types::StrongId<TextureDomainTag>;

    /**
     * @brief Handle type used to reference texture entities.
     */
    using TextureHandle = helios::ecs::types::EntityHandle<TextureDomainTag>;

}
