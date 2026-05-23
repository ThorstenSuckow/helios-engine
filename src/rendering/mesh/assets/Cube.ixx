/**
 * @file Cube.ixx
 * @brief Unit-cube mesh asset with positions, normals, UVs, and indexed triangles.
 */
module;

#include <span>
#include <array>

export module helios.engine.rendering.mesh.assets:Cube;

import helios.engine.rendering.common.types.Vertex;
import helios.engine.rendering.mesh.types.PrimitiveType;
import helios.engine.rendering.mesh.types.MeshData;

using namespace helios::engine::rendering::common::types;
using namespace helios::engine::rendering::mesh::types;
export namespace helios::engine::rendering::mesh::assets {

    /**
     * @brief Static cube mesh definition with per-face vertices.
     *
     * @details
     * The cube uses 24 vertices (4 per face) so each face can provide distinct
     * normals and texture coordinates. Indices define 12 triangles (36 indices).
     */
    struct Cube {

        /**
         * @brief Interleaved vertex data (position, normal, UV), grouped by face.
         */
        static constexpr std::array<Vertex, 24> vertices{{
            // n (z = 1.0)
            {{-1.0f, -1.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}}, // Bottom-left
            {{ 1.0f, -1.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 0.0f}}, // Bottom-right
            {{ 1.0f,  1.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}}, // Top-right
            {{-1.0f,  1.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 1.0f}}, // Top-left

            // f (z = -1.0)
            {{-1.0f, -1.0f, -1.0f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 0.0f}}, // Bottom-left
            {{ 1.0f, -1.0f, -1.0f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}}, // Bottom-right
            {{ 1.0f,  1.0f, -1.0f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 1.0f}}, // Top-right
            {{-1.0f,  1.0f, -1.0f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}}, // Top-left

            // l (x = -1.0)
            {{-1.0f, -1.0f, -1.0f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}}, // Bottom-left
            {{-1.0f, -1.0f,  1.0f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}}, // Bottom-right
            {{-1.0f,  1.0f,  1.0f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}}, // Top-right
            {{-1.0f,  1.0f, -1.0f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}}, // Top-left

            // r (x = 1.0)
            {{ 1.0f, -1.0f, -1.0f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}}, // Bottom-left
            {{ 1.0f, -1.0f,  1.0f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}}, // Bottom-right
            {{ 1.0f,  1.0f,  1.0f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}}, // Top-right
            {{ 1.0f,  1.0f, -1.0f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}}, // Top-left

            // b (y = -1.0)
            {{-1.0f, -1.0f, -1.0f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}}, // Bottom-left
            {{ 1.0f, -1.0f, -1.0f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f}}, // Bottom-right
            {{ 1.0f, -1.0f,  1.0f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}}, // Top-right
            {{-1.0f, -1.0f,  1.0f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}}, // Top-left

            // t (y = 1.0)
            {{-1.0f,  1.0f, -1.0f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 0.0f}}, // Bottom-left
            {{ 1.0f,  1.0f, -1.0f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}}, // Bottom-right
            {{ 1.0f,  1.0f,  1.0f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f}}, // Top-right
            {{-1.0f,  1.0f,  1.0f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}}  // Top-left
        }};


        /**
         * @brief Triangle index buffer (counter-clockwise winding).
         */
        static constexpr std::array<unsigned int, 36> indices = {
            // n
            0, 1, 2,    2, 3, 0,
            // f
            4, 5, 6,    6, 7, 4,
            // l
            8, 9, 10,   10, 11, 8,
            // r
            12, 13, 14, 14, 15, 12,
            // b
            16, 17, 18, 18, 19, 16,
            // t
            20, 21, 22, 22, 23, 20
        };

        /**
         * @brief Primitive topology used to interpret `indices`.
         */
        static constexpr helios::engine::rendering::mesh::types::PrimitiveType primitiveType = mesh::types::PrimitiveType::Triangles;

        /**
         * @brief Creates a lightweight `MeshData` view over the static cube buffers.
         *
         * @return `MeshData` referencing `vertices`, `indices`, and `primitiveType`.
         */
        static MeshData meshData() {
            return MeshData{
                std::span{vertices},
                std::span{indices},
                primitiveType
            };

        };

    };

}