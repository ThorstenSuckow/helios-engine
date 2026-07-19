module;

#include <cassert>

export module helios.engine.core.components.Vec2Component;

import helios.math.types;

import helios.math.concepts;

using namespace helios::math;
using namespace helios::math::concepts;
export namespace helios::engine::core::components {

    /**
     * @brief Generic 2D value component.
     *
     * @tparam TDomainTag Semantic domain tag.
     * @tparam THandle Owning entity handle type.
     * @tparam TNumericType Scalar type for vector values.
     */
    template<typename TDomainTag, typename TNumericType, typename ... Args>
    requires IsNumeric<TNumericType>
    class Vec2Component  {

        helios::math::vec2<TNumericType> value_{};

    public:

        using Value_type = helios::math::vec2<TNumericType>;

        Vec2Component() = default;

        /**
         * @brief Constructs the component with an initial value.
         *
         * @param value Initial value vector.
         */
        explicit Vec2Component(const vec2<TNumericType> value) : value_(value){}

        /**
         * @brief Copy constructor.
         *
         * @param other The component to copy from.
         */
        Vec2Component(const Vec2Component& other) :
            value_(other.value_) {}

        /** @brief Default copy assignment. */
        Vec2Component& operator=(const Vec2Component&) = default;
        /** @brief Default move constructor. */
        Vec2Component(Vec2Component&&) noexcept = default;
        /** @brief Default move assignment. */
        Vec2Component& operator=(Vec2Component&&) noexcept = default;

        /**
         * @brief Returns the current value.
         *
         * @return Current 2D vector value.
         */
        [[nodiscard]] vec2<TNumericType> value()  noexcept {
            return value_;
        }

        [[nodiscard]] vec2<TNumericType> value() const noexcept {
            return value_;
        }

        /**
         * @brief Updates the value.
         *
         * @param value New value vector.
         */
        void setValue(const vec2<TNumericType> value) noexcept {
            value_ = value;
        };


    };

}
