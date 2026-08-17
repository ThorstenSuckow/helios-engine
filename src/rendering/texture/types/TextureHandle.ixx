/**
* @file TextureHandle.ixx
 * @brief Entity-handle alias for viewport resources.
 */
module;

export module helios.engine.rendering.texture.types:TextureHandle;

import helios.ecs.common.types;

export namespace helios::engine::rendering::texture::types {
    
    struct TextureDomainTag{};

    /**
     * @brief Handle type used to reference texture entities.
     */
    using TextureHandle = helios::ecs::common::types::EntityHandle<TextureDomainTag>;

}
