/**
 * @file WireframeCube.ixx
 * @brief Simple wireframe cube mesh asset with positions, normals, UVs, and line indices.
 */
module;

#include <array>
#include <span>

export module helios.engine.rendering.mesh.assets:WireframeCube;

import helios.engine.rendering.common.types.Vertex;
import helios.engine.rendering.mesh.types.MeshData;
import helios.engine.rendering.mesh.types.PrimitiveType;
import helios.math;

using namespace helios::engine::rendering::common::types;
using namespace helios::engine::rendering::mesh::types;

export namespace helios::engine::rendering::mesh::assets {

    /**
     * @brief Static wireframe cube mesh definition.
     *
     * @details The cube is centered at the origin and spans from -0.5 to +0.5
     * on all three axes. It is rendered as line segments.
     */
    struct WireframeCube {

        /**
         * @brief Cube corner vertices.
         *
         * @details Normals and UVs are included to match the common Vertex layout,
         * but are not relevant for simple wireframe rendering.
         */
        static constexpr std::array<Vertex, 8> vertices{{
            {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}, // 0
            {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}}, // 1
            {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}, // 2
            {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}, // 3

            {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}, // 4
            {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}}, // 5
            {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}, // 6
            {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}  // 7
        }};

        /**
         * @brief Line index buffer.
         *
         * @details Every pair of indices describes one edge.
         */
        static constexpr std::array<unsigned int, 24> indices = {
            // back face
            0, 1,
            1, 2,
            2, 3,
            3, 0,

            // front face
            4, 5,
            5, 6,
            6, 7,
            7, 4,

            // connecting edges
            0, 4,
            1, 5,
            2, 6,
            3, 7
        };

        /**
         * @brief Primitive topology used to interpret `indices`.
         */
        static constexpr PrimitiveType primitiveType = PrimitiveType::Lines;

        /**
         * @brief Creates a lightweight `MeshData` view over the static cube buffers.
         * @return `MeshData` referencing `vertices`, `indices`, and `primitiveType`.
         */
        static MeshData meshData() {
            return MeshData{
                std::span{vertices},
                std::span{indices},
                primitiveType
            };
        }

        /**
         * @brief Returns the local bounds of this mesh.
         * @return local bounds as aabb
         */
        static helios::math::aabbf boundsData() {
            return helios::math::aabbf{
                -0.5f, -0.5f, -0.5f,
                 0.5f,  0.5f,  0.5f
            };
        }

    };

}