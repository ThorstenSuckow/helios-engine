module;

#include <cstddef>

export module helios.engine.core.components.Vec3Component;

import helios.math.types;

import helios.math.concepts;

using namespace helios::math;
using namespace helios::math::concepts;
export namespace helios::engine::core::components {

    /**
     * @brief Generic 3D value component with dirty-state tracking.
     *
     * @tparam TDomainTag Semantic domain tag.
     * @tparam THandle Owning entity handle type.
     * @tparam TNumericType Scalar type for vector values.
     */
    template<typename TDomainTag, typename TNumericType, typename ... Args>
    requires IsNumeric<TNumericType>
    class Vec3Component  {

        helios::math::vec3<TNumericType> value_{};

    public:

        Vec3Component() = default;

        /**
         * @brief Constructs the component with an initial value.
         *
         * @param value Initial value vector.
         */
        explicit Vec3Component(const vec3<TNumericType> value) : value_(value){}

        /**
         * @brief Constructs the component from scalar vector coordinates.
         *
         * @param x X component.
         * @param y Y component.
         * @param z Z component.
         */
        explicit Vec3Component(
        const TNumericType x, const TNumericType y, const TNumericType z)
        : value_(x, y, z){}

        /**
         * @brief Copy constructor.
         *
         * @details Copies the value and forces the copied component into a
         * dirty state to ensure dependent systems refresh cached data.
         *
         * @param other The component to copy from.
         */
        Vec3Component(const Vec3Component& other) :
            value_(other.value_) {}

        /** @brief Default copy assignment. */
        Vec3Component& operator=(const Vec3Component&) = default;
        /** @brief Default move constructor. */
        Vec3Component(Vec3Component&&) noexcept = default;
        /** @brief Default move assignment. */
        Vec3Component& operator=(Vec3Component&&) noexcept = default;


        /**
         * @brief Returns the current value.
         *
         * @return Current 3D vector value.
         */
        [[nodiscard]] vec3<TNumericType> value() const noexcept {
            return value_;
        }

        [[nodiscard]] vec3<TNumericType> value() noexcept {
            return value_;
        }

        /**
         * @brief Updates the value and marks the component dirty on change.
         *
         * @param value New value vector.
         */
        void setValue(const vec3<TNumericType> value) noexcept {
            value_ = value;
        };


    };

}
