/**
 * @file IsRenderResourceHandle.ixx
 * @brief Concept constraining render resource handle types.
 */
module;

#include <concepts>

export module helios.engine.rendering.common.concepts:IsRenderResourceHandle;

import helios.engine.rendering.shader.types;
import helios.engine.rendering.material.types;
import helios.engine.rendering.mesh.types;
import helios.engine.rendering.texture.types;

export namespace helios::engine::rendering::common::concepts {

    /**
     * @brief Constrains types to known render resource handles.
     *
     * @tparam T Type to validate.
     */
    template<typename T>
    concept IsRenderResourceHandle = std::is_same_v<T, shader::types::ShaderHandle> ||
                                     std::is_same_v<T, material::types::MaterialHandle> ||
                                     std::is_same_v<T, texture::types::TextureHandle> ||
                                     std::is_same_v<T, mesh::types::MeshHandle>;

}