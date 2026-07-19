/**
 * @file Mat4Component.ixx
 * @brief Generic 4x4 matrix ECS component with dirty-state tracking.
 */
module;

#include <cstddef>

export module helios.engine.core.components.Mat4Component;

import helios.math.types;

import helios.math.concepts;

using namespace helios::math;
using namespace helios::math::concepts;
export namespace helios::engine::core::components {

    /**
     * @brief Generic 4x4 matrix component with dirty-state tracking.
     *
     * @tparam TDomainTag Semantic domain tag.
     * @tparam THandle Owning entity handle type.
     * @tparam TNumericType Scalar type for matrix values.
     */
    template<typename TDomainTag, typename TNumericType, typename ... Args>
    requires IsNumeric<TNumericType>
    class Mat4Component  {

        helios::math::mat4<TNumericType> value_{};


    public:

        using Value_type = helios::math::mat4<TNumericType>;

        Mat4Component() = default;

        /**
         * @brief Constructs the component with an initial value.
         *
         * @param value Initial matrix value.
         */
        explicit Mat4Component(const Value_type& value) : value_(value) { }

        /**
         * @brief Constructs the component with a scalar-filled matrix.
         *
         * @param value Scalar used to initialize all matrix elements.
         */
        explicit Mat4Component(const TNumericType value) : value_(Value_type{value}) {}

        /**
         * @brief Copy constructor.
         *
         * @param other The component to copy from.
         *
         * @details Copies the value and forces the copied component into a
         * dirty state to ensure dependent systems refresh cached data.
         */
        Mat4Component(const Mat4Component& other) :
            value_(other.value_) {}

        /** @brief Default copy assignment. */
        Mat4Component& operator=(const Mat4Component&) = default;
        /** @brief Default move constructor. */
        Mat4Component(Mat4Component&&) noexcept = default;
        /** @brief Default move assignment. */
        Mat4Component& operator=(Mat4Component&&) noexcept = default;


        /**
         * @brief Returns the current value for in-place modification.
         *
         * @return Mutable reference to the current 4x4 matrix value.
         */
        [[nodiscard]] Value_type& value() noexcept {
            return value_;
        }

        /**
         * @brief Returns the current value.
         *
         * @return Current 4x4 matrix value.
         */
        [[nodiscard]] const Value_type& value() const noexcept {
            return value_;
        }

        /**
         * @brief Updates the value and marks the component dirty on change.
         *
         * @param value New matrix value.
         */
        void setValue(const Value_type& value) noexcept {
            value_ = value;
        };


    };

}

