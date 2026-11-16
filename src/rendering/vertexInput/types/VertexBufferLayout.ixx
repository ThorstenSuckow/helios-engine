/**
 * @file VertexAttributeLayout.ixx
 * @brief Declares GPU input layout information for one vertex attribute.
 */
module;

#include <cstdint>
#include <string>
#include <array>
#include <bitset>
#include <cstddef>
#include <utility>
#include <cassert>
#include "helios-engine-config.h"

export module helios.engine.rendering.vertexInput.types:VertexBufferLayout;

import helios.engine.rendering.common.types;

import :VertexAttributeSemantics;
import :VertexAttribute;
import :VertexInputRate;
import :VertexAttributeLayout;


export namespace helios::engine::rendering::vertexInput::types {


    class VertexBufferLayout {

        using Vertex = helios::engine::rendering::common::types::Vertex;


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
                && "VertexAttributeSemantics value out of range in VertexAttributeLayouts constructor");
#endif

            const auto idx = static_cast<std::size_t>(std::to_underlying(layout.attribute.semantics));

            layouts_[idx] = std::move(layout);
            active_.set(idx);
        }

        /**
         * @brief Byte stride between consecutive elements in the vertex buffer, expressed as VertexInputRate.
         */
        VertexInputRate inputRate_;

        /**
         * @brief Byte stride between consecutive elements.
         */
        std::uint32_t stride_;

    public:

        template<typename TVertex>
        static VertexBufferLayout from() {
            if constexpr (std::same_as<TVertex, Vertex>) {
                return VertexBufferLayout(
                    VertexInputRate::PerVertex,
                    sizeof(Vertex),
                    VertexAttributeLayout{VertexAttributeSemantics::Position, VertexAttributeType::Vec3f},
                    VertexAttributeLayout{VertexAttributeSemantics::Normal, VertexAttributeType::Vec3f},
                    VertexAttributeLayout{VertexAttributeSemantics::TextureCoordinates, VertexAttributeType::Vec2f}
                );
            } else if constexpr (std::same_as<TVertex, common::types::InstanceData>) {
                return VertexBufferLayout(
                    VertexInputRate::PerInstance,
                    sizeof(common::types::InstanceData),
                    VertexAttributeLayout{VertexAttributeSemantics::InstancedModelMatrix, VertexAttributeType::Mat4f},
                    VertexAttributeLayout{VertexAttributeSemantics::InstancedNormalizedAge, VertexAttributeType::Float}
                );
            } else {
                static_assert(false, "Unsupported Vertex type");
            }


        }

        [[nodiscard]] VertexInputRate inputRate() const noexcept {
            return inputRate_;
        }

        [[nodiscard]] std::uint32_t stride() const noexcept {
            return stride_;
        }

        VertexBufferLayout() noexcept = default;

        /**
         * @brief Constructs the component from one or more layout descriptors.
         *
         * @tparam TAttributes Parameter pack of `VertexAttributeLayout` values.
         * @param inputRate Byte stride between consecutive elements in the vertex buffer, expressed as VertexInputRate.
         * @param stride Byte stride between consecutive elements.
         * @param layout Layout descriptors to register as active.
         */
        template<typename ...TAttributes>
        requires (std::same_as<std::remove_cvref_t<TAttributes>, VertexAttributeLayout> && ...)
        explicit VertexBufferLayout(const VertexInputRate inputRate, const std::uint32_t stride, TAttributes&& ... layout)
            : inputRate_(inputRate), stride_(stride)
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
        [[nodiscard]] const auto& items() const noexcept {
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

    using InstanceDataLayout = VertexBufferLayout;

}