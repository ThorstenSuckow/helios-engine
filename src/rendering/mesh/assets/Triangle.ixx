/**
 * @file Triangle.ixx
 * @brief Simple triangle mesh asset with positions, normals, UVs, and indices.
 */
module;

#include <array>
#include <span>

export module helios.engine.rendering.mesh.assets:Triangle;

import helios.engine.rendering.common.types.Vertex;
import helios.engine.rendering.mesh.types.MeshData;
import helios.engine.rendering.mesh.types.PrimitiveType;

using namespace helios::engine::rendering::common::types;
using namespace helios::engine::rendering::mesh::types;
export namespace helios::engine::rendering::mesh::assets {

    /**
     * @brief Static triangle mesh definition.
     */
    struct Triangle {

        /**
         * @brief Interleaved vertex data (position, normal, UV).
         */
        static constexpr std::array<Vertex, 3> vertices{{
            {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
            {{ 1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
            {{ 0.0f,  1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.5f, 1.0f}}
        }};

        /**
         * @brief Triangle index buffer.
         */
        static constexpr std::array<unsigned int, 3> indices = {
            0, 1, 2
        };

        /**
         * @brief Primitive topology used to interpret `indices`.
         */
        static constexpr PrimitiveType primitiveType = PrimitiveType::Triangles;

        /**
         * @brief Creates a lightweight `MeshData` view over the static triangle buffers.
         * @return `MeshData` referencing `vertices`, `indices`, and `primitiveType`.
         */
        static MeshData meshData() {
            return MeshData{
                std::span{vertices},
                std::span{indices},
                primitiveType
            };
        }

    };

}

