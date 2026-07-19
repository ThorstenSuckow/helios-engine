/**
 * @file Vec4Component.ixx
 * @brief Generic 4D vector component.
 */
module;

#include <cassert>

export module helios.engine.core.components.Vec4Component;

import helios.math.types;

import helios.math.concepts;

using namespace helios::math;
using namespace helios::math::concepts;
export namespace helios::engine::core::components {

    /**
     * @brief Generic 4D value component.
     *
     * @tparam TDomainTag Semantic domain tag.
     * @tparam THandle Owning entity handle type.
     * @tparam TNumericType Scalar type for vector values.
     */
    template<typename TDomainTag, typename TNumericType, typename ... Args>
    requires IsNumeric<TNumericType>
    class Vec4Component  {

        helios::math::vec4<TNumericType> value_{};
    public:

        using Value_type = helios::math::vec4<TNumericType>;;

        Vec4Component() = default;

        /**
         * @brief Constructs the component with an initial value.
         *
         * @param value Initial value vector.
         */
        explicit Vec4Component(const Value_type value) : value_(value){}

        /**
         * @brief Constructs the component from scalar vector coordinates.
         *
         * @param x X component.
         * @param y Y component.
         * @param z Z component.
         * @param w W component.
         */
        explicit Vec4Component(
        const TNumericType x, const TNumericType y, const TNumericType z, const TNumericType w)
        : value_(x, y, z, w){}

        /**
         * @brief Copy constructor.
         *
         * @details Copies the value.
         *
         * @param other The component to copy from.
         */
        Vec4Component(const Vec4Component& other) :
            value_(other.value_) {}

        /** @brief Default copy assignment. */
        Vec4Component& operator=(const Vec4Component&) = default;
        /** @brief Default move constructor. */
        Vec4Component(Vec4Component&&) noexcept = default;
        /** @brief Default move assignment. */
        Vec4Component& operator=(Vec4Component&&) noexcept = default;

        /**
         * @brief Returns the current value.
         *
         * @return Current 4D vector value.
         */
        [[nodiscard]] Value_type value() const noexcept {
            return value_;
        }

        /**
         * @brief Returns the current value.
         *
         * @return Current 4D vector value.
         */
        [[nodiscard]] Value_type value() noexcept {
            return value_;
        }

        /**
         * @brief Updates the value.
         *
         * @param value New vector value.
         */
        void setValue(const Value_type value) noexcept {
            value_ = value;
        };


    };

}
