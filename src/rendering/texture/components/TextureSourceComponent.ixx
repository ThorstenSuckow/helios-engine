/**
 * @file TextureSourceComponent.ixx
 * @brief Component storing source paths for texture compilation.
 */
module;

#include <string>

export module helios.engine.rendering.texture.components:TextureSourceComponent;

import helios.engine.rendering.texture.concepts;


export namespace helios::engine::rendering::texture::components {

    /**
     * @brief Source-location component for texture entities.
     *
     * @tparam THandle Texture handle type.
     */
    template<typename THandle>
    requires texture::concepts::IsTextureHandle<THandle>
    struct TextureSourceComponent {

        /**
         * @brief Path to the texture source.
         */
        std::string texturePath;

    };

}