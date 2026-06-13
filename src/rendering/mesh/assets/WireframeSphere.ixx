/**
 * @file WireframeSphere.ixx
 * @brief Simple wireframe sphere mesh asset with positions, normals, UVs, and line indices.
 */
module;

#include <array>
#include <span>
#include <cmath>
#include <numbers>
#include <cstddef>

export module helios.engine.rendering.mesh.assets:WireframeSphere;

import helios.engine.rendering.common.types.Vertex;
import helios.engine.rendering.mesh.types.MeshData;
import helios.engine.rendering.mesh.types.PrimitiveType;
import helios.math;

using namespace helios::engine::rendering::common::types;
using namespace helios::engine::rendering::mesh::types;

export namespace helios::engine::rendering::mesh::assets {

    /**
     * @brief Static wireframe sphere mesh definition.
     *
     * @details The sphere is centered at the origin and has radius 0.5.
     * It is rendered as latitude and longitude line segments.
     */
    struct WireframeSphere {

        static constexpr std::size_t Stacks = 12;
        static constexpr std::size_t Slices = 24;

        static constexpr float Radius = 0.5f;

        static constexpr std::size_t VertexCount =
            (Stacks + 1) * Slices;

        static constexpr std::size_t HorizontalEdgeCount =
            (Stacks - 1) * Slices;

        static constexpr std::size_t VerticalEdgeCount =
            Stacks * Slices;

        static constexpr std::size_t IndexCount =
            2 * (HorizontalEdgeCount + VerticalEdgeCount);

    private:

        static std::size_t vertexIndex(
            const std::size_t stack,
            const std::size_t slice
        ) noexcept {
            return stack * Slices + slice;
        }

        static std::array<Vertex, VertexCount> makeVertices() {
            std::array<Vertex, VertexCount> result{};

            constexpr float pi = std::numbers::pi_v<float>;

            for (std::size_t stack = 0; stack <= Stacks; ++stack) {
                const float v = static_cast<float>(stack) / static_cast<float>(Stacks);
                const float theta = v * pi;

                const float y = std::cos(theta) * Radius;
                const float ringRadius = std::sin(theta) * Radius;

                for (std::size_t slice = 0; slice < Slices; ++slice) {
                    const float u = static_cast<float>(slice) / static_cast<float>(Slices);
                    const float phi = u * 2.0f * pi;

                    const float x = std::cos(phi) * ringRadius;
                    const float z = std::sin(phi) * ringRadius;

                    const float nx = x / Radius;
                    const float ny = y / Radius;
                    const float nz = z / Radius;

                    result[vertexIndex(stack, slice)] = Vertex{
                        {x, y, z},
                        {nx, ny, nz},
                        {u, v}
                    };
                }
            }

            return result;
        }

        static std::array<unsigned int, IndexCount> makeIndices() {
            std::array<unsigned int, IndexCount> result{};
            std::size_t cursor = 0;

            // Latitude rings, excluding top and bottom poles.
            for (std::size_t stack = 1; stack < Stacks; ++stack) {
                for (std::size_t slice = 0; slice < Slices; ++slice) {
                    const auto current = vertexIndex(stack, slice);
                    const auto next = vertexIndex(stack, (slice + 1) % Slices);

                    result[cursor++] = static_cast<unsigned int>(current);
                    result[cursor++] = static_cast<unsigned int>(next);
                }
            }

            // Longitude arcs.
            for (std::size_t slice = 0; slice < Slices; ++slice) {
                for (std::size_t stack = 0; stack < Stacks; ++stack) {
                    const auto current = vertexIndex(stack, slice);
                    const auto next = vertexIndex(stack + 1, slice);

                    result[cursor++] = static_cast<unsigned int>(current);
                    result[cursor++] = static_cast<unsigned int>(next);
                }
            }

            return result;
        }

    public:

        /**
         * @brief Interleaved vertex data.
         */
        inline static const std::array<Vertex, VertexCount> vertices = makeVertices();

        /**
         * @brief Line index buffer.
         */
        inline static const std::array<unsigned int, IndexCount> indices = makeIndices();

        /**
         * @brief Primitive topology used to interpret `indices`.
         */
        static constexpr PrimitiveType primitiveType = PrimitiveType::Lines;

        /**
         * @brief Creates a lightweight `MeshData` view over the static sphere buffers.
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
                -Radius, -Radius, -Radius,
                 Radius,  Radius,  Radius
            };
        }

    };

}