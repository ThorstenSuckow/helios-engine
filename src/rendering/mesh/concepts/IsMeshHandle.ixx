/**
 * @file IsMeshHandle.ixx
 * @brief Concept constraining shader handle types.
 */
module;

#include <concepts>

export module helios.engine.rendering.mesh.concepts.IsMeshHandle;

import helios.engine.rendering.mesh.types.MeshHandle;

using namespace helios::engine::rendering::mesh::types;
export namespace helios::engine::rendering::mesh::concepts {

    /**
     * @brief Constrains a type to the mesh handle alias.
     *
     * @tparam T Type to validate.
     */
    template<class T>
    concept IsMeshHandle = std::is_same_v<T, MeshHandle>;

}