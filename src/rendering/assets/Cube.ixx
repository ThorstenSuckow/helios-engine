/**
 * @file Cube.ixx
 * @brief Basic cube shape for rendering.
 */
module;


#include <vector>

export module helios.engine.rendering.assets:Cube;

import helios.engine.rendering.Vertex;
import helios.engine.rendering.mesh.types.PrimitiveType;

using namespace helios::engine::rendering;

export namespace helios::engine::rendering::assets {

    /**
     * @brief Basic Cube Shape.
     */
    struct Cube {

        std::vector<Vertex> vertices{
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
        };


        std::vector<unsigned int> indices = {
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

        helios::engine::rendering::mesh::types::PrimitiveType primitiveType = mesh::types::PrimitiveType::LineLoop;

    };

}