/**
 * @file IsRenderResourceHandle.ixx
 * @brief Concept constraining render resource handle types.
 */
module;

#include <concepts>

export module helios.engine.rendering.common.concepts.IsRenderResourceHandle;

import helios.engine.rendering.shader.types.ShaderHandle;
import helios.engine.rendering.material.types.MaterialHandle;
import helios.engine.rendering.mesh.types.MeshHandle;

using namespace helios::engine::rendering::shader::types;
using namespace helios::engine::rendering::material::types;
using namespace helios::engine::rendering::mesh::types;
export namespace helios::engine::rendering::common::concepts {

    /**
     * @brief Constrains types to known render resource handles.
     *
     * @tparam T Type to validate.
     */
    template<typename T>
    concept IsRenderResourceHandle = std::is_same_v<T, ShaderHandle> ||
                                     std::is_same_v<T, MaterialHandle> ||
                                     std::is_same_v<T, MeshHandle>;

}