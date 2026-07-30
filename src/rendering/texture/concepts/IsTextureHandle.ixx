/**
 * @file IsTextureHandle.ixx
 * @brief Concept constraining texture handle types.
 */
module;

#include <concepts>

export module helios.engine.rendering.texture.concepts:IsTextureHandle;

import helios.engine.rendering.texture.types;

export namespace helios::engine::rendering::texture::concepts {

    /**
     * @brief Constrains a type to the texture handle alias.
     *
     * @tparam T Type to validate.
     */
    template<class T>
    concept IsTextureHandle = std::is_same_v<T, texture::types::TextureHandle>;

}