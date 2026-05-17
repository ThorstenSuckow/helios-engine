/**
 * @file Triangle.ixx
 * @brief Basic Triangle (line) shape for rendering.
 */
module;


#include <vector>

export module helios.engine.rendering.asset.shape.basic.Triangle;

import helios.engine.rendering.asset.shape.Shape;
import helios.engine.rendering.Vertex;
import helios.engine.rendering.mesh.types.PrimitiveType;

using namespace helios::engine::rendering;

export namespace helios::engine::rendering::asset::shape::basic {

    /**
     * @brief Basic Triangle Shape.
     *
     * Creates vertex and index data for a single Triangle line segment. By default, this shape
     * defines three vertices  (-1, -1, 0), (-1, 1, 0) to the point (1, 0, 0).
     */
    struct Triangle : public Shape {

        Triangle(): Shape(
            std::vector<Vertex>{

                {{ -0.5f,  -0.5f,  0.0f}, { 0.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
                {{ -0.5f,  0.5f,  0.0f}, { 0.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
                {{ 0.5f,  0.0f,  0.0f}, { 0.0f,  0.0f,  0.0f}, {0.0f, 0.0f}}
            },
            std::vector<unsigned int>{
                0, 1, 2, 0
            }
        ) {}

        [[nodiscard]] helios::engine::rendering::mesh::types::PrimitiveType primitiveType() const noexcept override {
            return mesh::types::PrimitiveType::Lines;
        }

    };

}