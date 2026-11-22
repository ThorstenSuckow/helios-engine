/**
 * @file ShaderAttributeMapping.ixx
 * @brief Type that maps an attribute semantic to a shader vertex attribute name.
 */
module;

#include <string>
#include <array>
#include <cstddef>
#include <utility>
#include <cassert>
#include <bitset>
#include "helios-engine-config.h"

export module helios.engine.rendering.shader.types:ShaderAttributeMappings;

import helios.engine.rendering.vertexInput.types;

export namespace helios::engine::rendering::shader::types {
    /**
     * @brief Mapping entry from `UniformSemantics` to a concrete uniform name and location..
     */
    struct ShaderAttributeMapping {

        /**
         * @brief Semantic key used by runtime systems.
         */
        vertexInput::types::VertexAttributeSemantics semantics;

        /**
         * @brief Vertex attribute variable name expected by the shader program.
         */
        std::string name;

        unsigned int location;
    };

    class ShaderAttributeMappings {

        /**
         * @brief Number of supported vertex attribute semantics.
         */
        static constexpr auto VertexAttributeSemanticsCount = static_cast<std::size_t>(
            std::to_underlying(vertexInput::types::VertexAttributeSemantics::size_)
        );

        /**
         * @brief Mapping table from semantic index to vertex attribute name.
         */
        std::array<ShaderAttributeMapping, VertexAttributeSemanticsCount> items_{};
        std::bitset<VertexAttributeSemanticsCount> active_;

;        void registerMapping(ShaderAttributeMapping&& mapping) {
            const auto idx = std::to_underlying(mapping.semantics);
            items_[idx] = std::move(mapping);
            active_.set(idx);
        }

    public:


        /**
         * @brief Constructs mappings from one or more semantic/name entries.
         * @tparam TMappings Mapping entry types compatible with `ShaderAttributeMapping`.
         * @param mapping Mapping entries assigned into `mappings` by semantic index.
         * @details
         * The constructor asserts that each semantic value is within the valid
         * `[0, VertexAttributeSemanticsCount)` range.
         */
        template<typename ...TMappings>
        requires (std::same_as<std::remove_cvref_t<TMappings>, ShaderAttributeMapping> && ...)
        explicit ShaderAttributeMappings(TMappings&& ... mapping)
        {
#if HELIOS_DEBUG
            (assert(std::to_underlying(mapping.semantics) < VertexAttributeSemanticsCount
                && "VertexAttributeSemantics value out of range in ShaderAttributeMappings constructor"), ...);
#endif

            (registerMapping(std::move(mapping)), ...);
        }


        [[nodiscard]] const std::array<ShaderAttributeMapping, VertexAttributeSemanticsCount>& items() const noexcept {
            return items_;
        };

        [[nodiscard]] std::bitset<VertexAttributeSemanticsCount> active() const noexcept {
            return active_;
        }

    };
}