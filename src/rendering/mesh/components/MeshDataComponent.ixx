/**
 * @file MeshDataComponent.ixx
 * @brief ECS component that provides raw MeshData.
 */
module;

#include <utility>
#include <concepts>

export module helios.engine.rendering.mesh.components.MeshDataComponent;

import helios.engine.rendering.mesh.types.MeshData;

using namespace helios::engine::rendering::mesh::types;
export namespace helios::engine::rendering::mesh::components {

    /**
     * @brief Component that provides raw MeshData.
     */
    template<typename TOwnerHandle>
    struct MeshDataComponent {

        MeshData meshData;

        template<typename ... TArgs>
        requires std::constructible_from<MeshData, TArgs...>
        explicit MeshDataComponent(TArgs&& ... args)
        : meshData(std::forward<TArgs>(args)...)
        {}

    };


}