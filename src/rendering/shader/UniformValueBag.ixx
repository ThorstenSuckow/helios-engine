/**
 * @file UniformValueBag.ixx
 * @brief Scope-local, typed storage for shader uniform values.
 */
module;

#include <array>
#include <bitset>
#include <cassert>
#include <utility>
#include <variant>

export module helios.engine.rendering.shader.UniformValueBag;

import helios.math.types;

import helios.engine.rendering.shader.types.UniformSemantics;
import helios.engine.rendering.shader.types.uniforms;


using namespace helios::engine::rendering::shader::types;
export namespace helios::engine::rendering::shader {

    /**
     * @brief Typed uniform container keyed by compile-time uniform tags.
     * @tparam TUniformScope Uniform lifetime scope tag (for example pass or draw).
     */
    template<typename TUniformScope>
    class UniformValueBag {
        helios::math::mat4f modelMatrix;
        helios::math::mat4f projectionMatrix;
        helios::math::mat4f viewMatrix;
        helios::math::vec4f materialBaseColor;

        helios::math::vec4f textColor;

        int textTexture;

        float materialRoughness;
        float deltaTime;
        float totalTime;

        std::bitset<std::to_underlying(UniformSemantics::size_)> active_;

        public :

        /**
         * @brief Constructs a value bag and resets all stored values.
         */
        UniformValueBag() {
            clearValues();
        }


        /**
         * @brief Resets all stored uniform values to defaults.
         */
        void clearValues() {
            modelMatrix = helios::math::mat4f();
            projectionMatrix = helios::math::mat4f();
            viewMatrix = helios::math::mat4f();
            materialBaseColor = helios::math::vec4f();
            textColor = helios::math::vec4f();

            textTexture = 0;

            materialRoughness = 0.0f;
            deltaTime = 0.0f;
            totalTime = 0.0f;
        }

        /**
         * @brief Checks whether a typed uniform value is marked as active.
         * @tparam TUniform Uniform tag type providing `semantics`.
         * @return `true` if the semantic slot is active.
         */
        template<typename TUniform>
        [[nodiscard]] bool has() const noexcept {
            return active_.test(std::to_underlying(TUniform::semantics));
        }

        /**
         * @brief Checks whether a semantic slot is marked as active.
         * @param semantics Semantic slot to query.
         * @return `true` if the semantic slot is active.
         */
        [[nodiscard]] bool has(const UniformSemantics semantics) const noexcept {
            return active_.test(std::to_underlying(semantics));
        }

        /**
         * @brief Returns the stored value for a typed uniform.
         * @tparam TUniform Uniform tag type.
         * @return Const reference to the stored value.
         */
        template<typename TUniform>
        [[nodiscard]] const typename TUniform::value_type& get() {
            if constexpr (std::same_as<TUniform, ModelMatrixUniform>) {
                return modelMatrix;
            } else if constexpr (std::same_as<TUniform, ViewMatrixUniform>) {
                return viewMatrix;
            } else if constexpr (std::same_as<TUniform, ProjectionMatrixUniform>) {
                return projectionMatrix;
            } else if constexpr (std::same_as<TUniform, MaterialBaseColorUniform>) {
                return materialBaseColor;
            } else {
                assert(false && "Unsupported uniform type in UniformValueBag::value");
            }


        }

        /**
         * @brief Stores a value for a typed uniform and marks it active.
         * @tparam TUniform Uniform tag type.
         * @param value Value to store.
         */
        template<typename TUniform>
        void set (const typename TUniform::value_type& value) {
            if constexpr (std::same_as<TUniform, ModelMatrixUniform>) {
                modelMatrix = value;
            } else if constexpr (std::same_as<TUniform, ViewMatrixUniform>) {
                viewMatrix = value;
            } else if constexpr (std::same_as<TUniform, ProjectionMatrixUniform>) {
                projectionMatrix = value;
            } else if constexpr (std::same_as<TUniform, MaterialBaseColorUniform>) {
                materialBaseColor = value;
            } else {
                assert(false && "Unsupported uniform type in UniformValueBag::value");
            }

            active_.set(std::to_underlying(TUniform::semantics));
        }

        /**
         * @brief Returns a matrix pointer for a matrix semantic.
         * @param semantics Semantic slot to query.
         * @return Pointer to `mat4f` or `nullptr`.
         */
        [[nodiscard]] const helios::math::mat4f* mat4f(const UniformSemantics semantics) const noexcept {
            if (!has(semantics)) {
                return nullptr;
            }

            switch (semantics) {
                case UniformSemantics::ModelMatrix:
                    return &modelMatrix;
                case UniformSemantics::ViewMatrix:
                    return &viewMatrix;
                case UniformSemantics::ProjectionMatrix:
                    return &projectionMatrix;
                default:
                    assert(false && "Unsupported uniform type in UniformValueBag::value");
            }
        }

        /**
         * @brief Returns a vector pointer for a vector semantic.
         * @param semantics Semantic slot to query.
         * @return Pointer to `vec4f` or `nullptr`.
         */
        [[nodiscard]] const helios::math::vec4f* vec4f(const UniformSemantics semantics) const noexcept {
            if (!has(semantics)) {
                return nullptr;
            }

            switch (semantics) {
            case UniformSemantics::MaterialBaseColor:
                return &materialBaseColor;
            case UniformSemantics::TextColor:
                return &textColor;
            default:
                assert(false && "Unsupported uniform type in UniformValueBag::value");
            }
        }
    };

}