/**
 * @file UniformMappingsComponent.ixx
 * @brief Component that stores semantic-to-uniform-name mappings for shader usage.
 */
module;

#include <string>
#include <array>
#include <cstddef>
#include <utility>
#include <cassert>

export module helios.engine.rendering.shader.components.UniformMappingsComponent;

import helios.engine.rendering.shader.types.UniformSemantics;
import helios.engine.rendering.shader.types.UniformMapping;

using namespace helios::engine::rendering::shader::types;
export namespace helios::engine::rendering::shader::components {

    /**
     * @brief Stores uniform names indexed by `UniformSemantics`.
     * @tparam TOwnerHandle Owner handle type used by ECS composition.
     */
    template<typename TOwnerHandle, typename TUniformScope>
    struct UniformMappingsComponent {

        /**
         * @brief Number of supported uniform semantics.
         */
        static constexpr auto UniformSemanticsCount = static_cast<std::size_t>(
            std::to_underlying(UniformSemantics::size_)
        );

        /**
         * @brief Mapping table from semantic index to uniform name.
         */
        std::array<std::string, UniformSemanticsCount> mappings{};

    public:
        /**
         * @brief Constructs mappings from one or more semantic/name entries.
         * @tparam TMappings Mapping entry types compatible with `UniformMapping`.
         * @param mapping Mapping entries assigned into `mappings` by semantic index.
         * @details
         * The constructor asserts that each semantic value is within the valid
         * `[0, UniformSemanticsCount)` range.
         */
        template<typename ...TMappings>
        requires (std::same_as<std::remove_cvref_t<TMappings>, UniformMapping> && ...)
        explicit UniformMappingsComponent(TMappings&& ... mapping)
        {
            (assert(std::to_underlying(mapping.semantics) < UniformSemanticsCount
                && "UniformSemantics value out of range in UniformMappingsComponent constructor"), ...);

            ((mappings[std::to_underlying(mapping.semantics)] = std::move(mapping.name)), ...);
        }



    };

}