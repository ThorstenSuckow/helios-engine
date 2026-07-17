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

        size_t previousVersion_ = 0;
        size_t currentVersion_ = 0;

    public:

        AABBComponent() = default;

        /**
         * @brief Constructs the component with an initial value.
         *
         * @param value Initial value vector.
         */
        explicit AABBComponent(const aabb<TNumericType>& value) : value_(value), currentVersion_(1) {}

        /**
         * @brief Copy constructor.
         *
         * @details Copies the value and forces the copied component into a
         * dirty state to ensure dependent systems refresh cached data.
         *
         * @param other The component to copy from.
         */
        AABBComponent(const AABBComponent& other) :
            value_(other.value_),
            previousVersion_(0),
            currentVersion_(1) {}

        /** @brief Default copy assignment. */
        AABBComponent& operator=(const AABBComponent&) = default;
        /** @brief Default move constructor. */
        AABBComponent(AABBComponent&&) noexcept = default;
        /** @brief Default move assignment. */
        AABBComponent& operator=(AABBComponent&&) noexcept = default;

        /**
         * @brief Lifecycle hook called when the component is acquired.
         *
         * @details Marks the component dirty to trigger downstream recomputation.
         */
        void onAcquire() noexcept {
            currentVersion_++;
        }

        /**
         * @brief Lifecycle hook called when the component is released.
         *
         * @details Marks the component dirty to trigger downstream recomputation.
         */
        void onRelease() noexcept {
            currentVersion_++;
        }

        /**
         * @brief Clears the dirty flag after dependent systems consumed updates.
         */
        void commit() noexcept {
            previousVersion_ = currentVersion_;
        }

        /**
         * @brief Returns whether the component requires a refresh pass.
         *
         * @return `true` if value changed or lifecycle hooks marked dirty.
         */
        [[nodiscard]] bool hasChanges() const noexcept {
            return previousVersion_ != currentVersion_;
        }

        /**
         * @brief Returns the current value.
         *
         * @return Current axis-aligned bounding box value.
         */
        [[nodiscard]] aabb<TNumericType>& value() noexcept {
            return value_;
        }

        [[nodiscard]] const aabb<TNumericType>& value() const noexcept {
            return value_;
        }

        /**
         * @brief Updates the value and marks the component dirty on change.
         *
         * @param value New value vector.
         */
        void setValue(const aabb<TNumericType> value) noexcept {
            value_ = value;
            currentVersion_++;
        };


    };

}


