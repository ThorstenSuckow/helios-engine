module;

#include <cstddef>

export module helios.engine.core.components.AABBComponent;

import helios.math;

using namespace helios::math;
using namespace helios::math::concepts;
export namespace helios::engine::core::components {

    /**
     * @brief Generic AABB value component with dirty-state tracking.
     *
     * @tparam TDomainTag Semantic domain tag.
     * @tparam THandle Owning entity handle type.
     * @tparam TNumericType Scalar type for AABB values.
     */
    template<typename TDomainTag, typename TNumericType, typename ... Args>
    requires IsNumeric<TNumericType>
    class AABBComponent  {

        helios::math::aabb<TNumericType> value_{};

    public:

        using Value_type = helios::math::aabb<TNumericType>;

        AABBComponent() = default;

        /**
         * @brief Constructs the component with an initial value.
         *
         * @param value Initial value vector.
         */
        explicit AABBComponent(const Value_type& value) : value_(value) {}

        /**
         * @brief Copy constructor.
         *
         * @details Copies the value and forces the copied component into a
         * dirty state to ensure dependent systems refresh cached data.
         *
         * @param other The component to copy from.
         */
        AABBComponent(const AABBComponent& other) :
            value_(other.value_) {}

        /** @brief Default copy assignment. */
        AABBComponent& operator=(const AABBComponent&) = default;
        /** @brief Default move constructor. */
        AABBComponent(AABBComponent&&) noexcept = default;
        /** @brief Default move assignment. */
        AABBComponent& operator=(AABBComponent&&) noexcept = default;


        /**
         * @brief Returns the current value.
         *
         * @return Current axis-aligned bounding box value.
         */
        [[nodiscard]] Value_type& value() noexcept {
            return value_;
        }

        [[nodiscard]] const Value_type& value() const noexcept {
            return value_;
        }

        /**
         * @brief Updates the value and marks the component dirty on change.
         *
         * @param value New value vector.
         */
        void setValue(const Value_type& value) noexcept {
            value_ = value;
        };


    };

}


