/**
 * @file VertexAttributeLayoutComponent.ixx
 * @brief Component that stores active vertex attribute layouts by semantic.
 */
module;

#include <string>
#include <array>
#include <bitset>
#include <cstddef>
#include <utility>
#include <cassert>
#include "helios-engine-config.h"

export module helios.engine.rendering.mesh.components.VertexAttributeLayoutComponent;

import helios.engine.rendering.mesh.types.VertexAttributeSemantics;
import helios.engine.rendering.mesh.types.VertexAttributeLayout;
import helios.engine.rendering.mesh.types.VertexAttribute;

using namespace helios::engine::rendering::mesh::types;
export namespace helios::engine::rendering::mesh::components {


    /**
     * @brief Stores a semantic-indexed set of vertex attribute layouts.
     *
     * @tparam TOwnerHandle Owning entity handle type.
     * @tparam TVertexInput Marker type describing the input stepping domain.
     */
    template<typename TOwnerHandle, typename TVertexInput>
    class VertexAttributeLayoutComponent {

        /**
         * @brief Number of semantic slots available in the layout table.
         */
        static constexpr auto VertexAttributeSemanticsCount = static_cast<std::size_t>(
            std::to_underlying(VertexAttributeSemantics::size_)
        );

        /**
         * @brief Semantic-indexed storage for attribute layouts.
         */
        std::array<VertexAttributeLayout, VertexAttributeSemanticsCount> layouts_{};

        /**
         * @brief Activity mask indicating which semantic slots are populated.
         */
        std::bitset<VertexAttributeSemanticsCount> active_;

        /**
         * @brief Registers or replaces one layout entry and marks it active.
         *
         * @param layout Layout descriptor to store.
         */
        void set(VertexAttributeLayout layout) {
#if HELIOS_DEBUG
            assert(std::to_underlying(layout.attribute.semantics) < VertexAttributeSemanticsCount
                && "VertexAttributeSemantics value out of range in VertexAttributeLayoutComponent constructor");
#endif

            const auto idx = static_cast<std::size_t>(std::to_underlying(layout.attribute.semantics));

            layouts_[idx] = std::move(layout);
            active_.set(idx);
        }

    public:

        /**
         * @brief Constructs the component from one or more layout descriptors.
         *
         * @tparam TAttributes Parameter pack of `VertexAttributeLayout` values.
         * @param layout Layout descriptors to register as active.
         */
        template<typename ...TAttributes>
        requires (std::same_as<std::remove_cvref_t<TAttributes>, VertexAttributeLayout> && ...)
        explicit VertexAttributeLayoutComponent(TAttributes&& ... layout)
        {
            (set(std::forward<TAttributes>(layout)), ...);
        }


        /**
         * @brief Returns the layout for a semantic, or `nullptr` if not active.
         *
         * @param semantics Semantic to query.
         * @return Pointer to layout if active; otherwise `nullptr`.
         */
        [[nodiscard]] const VertexAttributeLayout* layout(const VertexAttributeSemantics semantics) const noexcept {

            const auto idx = static_cast<std::size_t>(std::to_underlying(semantics));
            if (!active_.test(idx)) {
                return nullptr;
            }

            return &layouts_[idx];
        }

        /**
         * @brief Returns the internal semantic-indexed layout storage.
         *
         * @return Read-only reference to all layout slots.
         */
        [[nodiscard]] const auto& layouts() const noexcept {
            return layouts_;
        }

        /**
         * @brief Returns the semantic activity mask.
         *
         * @return Bitset with one bit per `VertexAttributeSemantics` slot.
         */
        [[nodiscard]] const auto& active() const noexcept {
            return active_;
        }

    };

}