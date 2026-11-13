/**
 * @file VertexAttributeMapping.ixx
 * @brief Type that maps an attribute semantic to a shader vertex attribute name.
 */
module;

#include <string>
#include <array>
#include <cstddef>
#include <utility>
#include <cassert>
#include "helios-engine-config.h"

export module helios.engine.rendering.shader.types.VertexAttributeMappings;

import helios.engine.rendering.mesh.types.VertexAttributeSemantics;

export namespace helios::engine::rendering::shader::types {

    /**
     * @brief Mapping entry from `UniformSemantics` to a concrete uniform name and location..
     */
    struct VertexAttributeMapping {

        /**
         * @brief Semantic key used by runtime systems.
         */
        mesh::types::VertexAttributeSemantics semantics;

        /**
         * @brief Vertex attribute variable name expected by the shader program.
         */
        std::string name;

        unsigned int location;
    };

    struct VertexAttributeMappings {

        using VertexAttributeSemantics = helios::engine::rendering::mesh::types::VertexAttributeSemantics;
        using VertexAttributeMapping = helios::engine::rendering::shader::types::VertexAttributeMapping;

        /**
         * @brief Number of supported vertex attribute semantics.
         */
        static constexpr auto VertexAttributeSemanticsCount = static_cast<std::size_t>(
            std::to_underlying(VertexAttributeSemantics::size_)
        );

        /**
         * @brief Mapping table from semantic index to vertex attribute name.
         */
        std::array<VertexAttributeMapping, VertexAttributeSemanticsCount> mappings{};

    public:


        /**
         * @brief Constructs mappings from one or more semantic/name entries.
         * @tparam TMappings Mapping entry types compatible with `VertexAttributeMapping`.
         * @param mapping Mapping entries assigned into `mappings` by semantic index.
         * @details
         * The constructor asserts that each semantic value is within the valid
         * `[0, VertexAttributeSemanticsCount)` range.
         */
        template<typename ...TMappings>
        requires (std::same_as<std::remove_cvref_t<TMappings>, VertexAttributeMapping> && ...)
        explicit VertexAttributeMappings(TMappings&& ... mapping)
        {
#if HELIOS_DEBUG
            (assert(std::to_underlying(mapping.semantics) < VertexAttributeSemanticsCount
                && "VertexAttributeSemantics value out of range in VertexAttributeMappings constructor"), ...);
#endif

            ((mappings[std::to_underlying(mapping.semantics)] = std::move(mapping)), ...);
        }



    };

}