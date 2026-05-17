/**
 * @file Line.ixx
 * @brief Basic line shape for rendering.
 */
module;


#include <vector>

export module helios.engine.rendering.asset.shape.basic.Line;

import helios.engine.rendering.asset.shape.Shape;
import helios.engine.rendering.Vertex;
import helios.engine.rendering.mesh.types.PrimitiveType;

using namespace helios::engine::rendering;

export namespace helios::engine::rendering::asset::shape::basic {

    /**
     * @brief Basic Line Shape.
     *
     * Creates vertex and index data for a single diagonal line segment. By default, this shape
     * defines a line spanning from (0, 0, 0) to the point (1, 1, 0).
     */
    struct Line : public Shape {

        Line(): Shape(
            std::vector<Vertex>{

                {{ 0.0f,  0.0f,  0.0f}, { 0.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
                {{ 1.0f,  1.0f,  0.0f}, { 0.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
            },
            std::vector<unsigned int>{
                0, 1
            }
        ) {}

        [[nodiscard]] helios::engine::rendering::mesh::types::PrimitiveType primitiveType() const noexcept override {
            return mesh::types::PrimitiveType::Lines;
        }

    };

}