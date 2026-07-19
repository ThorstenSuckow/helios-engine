/**
 * @file QuaternionComponent.ixx
 * @brief Generic quaternion ECS component with dirty-state tracking.
 */
module;

#include <concepts>
#include <cstddef>

export module helios.engine.core.components.QuaternionComponent;

import helios.math;


export namespace helios::engine::core::components {

    /**
     * @brief Generic quaternion component with domain tagging and dirty tracking.
     *
     * @tparam TDomainTag Domain tag type used for semantic component grouping.
     * @tparam TNumericType Floating-point scalar type used by `helios::math::quat`.
     * @tparam Args Additional template arguments (e.g. owner handle, tags).
     */
    template<typename TDomainTag, typename TNumericType, typename ... Args>
    requires std::floating_point<TNumericType>
    class QuaternionComponent {

        helios::math::quat<TNumericType> quat = helios::math::quat<TNumericType>::identity();

    public:

        using Value_type = helios::math::quat<TNumericType>;

        /**
         * @brief Returns the current quaternion value.
         *
         * @return Const reference to the stored quaternion.
         */
        [[nodiscard]] const Value_type& value() const noexcept {
            return quat;
        }

        /**
         * @brief Updates the quaternion value and marks the component as dirty.
         *
         * @param value New quaternion value.
         */
        void setValue(const Value_type& value) noexcept {
            quat = value;
        }


    };

}
