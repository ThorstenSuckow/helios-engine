/**
 * @file NumericValueComponent.ixx
 * @brief Generic scalar component.
 */
module;

#include <cassert>
#include <algorithm>
#include <cmath>

export module helios.engine.core.components.NumericValueComponent;

import helios.math;

using namespace helios::math;
using namespace helios::math::concepts;
export namespace helios::engine::core::components {

    /**
     * @brief Generic numeric value component.
     *
     * @tparam TDomainTag Semantic domain tag.
     * @tparam THandle Owning entity handle type.
     * @tparam TNumericType Scalar type for stored numeric values.
     */
    template<typename TDomainTag, typename TNumericType, typename ... Args>
    requires IsNumeric<TNumericType>
    class NumericValueComponent  {

        TNumericType value_{};

    public:

        using Value_type = TNumericType;

        NumericValueComponent() = default;


        /**
         * @brief Constructs the component with an initial scalar value.
         *
         * @param value Initial numeric value.
         */
        explicit NumericValueComponent(const TNumericType value) : value_(value){}

        /**
         * @brief Copy constructor.
         *
         * @param other The component to copy from.
         */
        NumericValueComponent(const NumericValueComponent& other) :
            value_(other.value_){}

        /** @brief Default copy assignment. */
        NumericValueComponent& operator=(const NumericValueComponent&) = default;
        /** @brief Default move constructor. */
        NumericValueComponent(NumericValueComponent&&) noexcept = default;
        /** @brief Default move assignment. */
        NumericValueComponent& operator=(NumericValueComponent&&) noexcept = default;


        /**
         * @brief Returns the current value.
         *
         * @return Current value.
         */
        [[nodiscard]] TNumericType value() noexcept {
            return value_;
        }

        /**
         * @brief Returns the current value.
         *
         * @return Current value.
         */
        [[nodiscard]] TNumericType value() const noexcept {
            return value_;
        }

        /**
         * @brief Updates the value.
         *
         * @param value New scalar value.
         */
        void setValue(const TNumericType value) noexcept {
            value_ = value;
        };


    };

}
