/**
 * @file YawPitchRollComponent.ixx
 * @brief Component storing yaw/pitch/roll angles in radians.
 */
module;

export module helios.engine.spatial.components.YawPitchRollComponent;

import helios.engine.runtime.world.GameObject;
import helios.math.types;

import helios.engine.core.spatial;
import helios.engine.core.units.Unit;

export namespace helios::engine::spatial::components {

    /**
     * @brief Stores Euler-style camera/control angles in radians.
     *
     * @tparam THandle Owning entity handle type.
     */
    template<typename THandle>
    struct YawPitchRollComponent {

        using Value_type = helios::math::vec3f;

        /** @brief Rotation around up axis in radians. */
        float yaw = 0;

        /** @brief Rotation around right axis in radians. */
        float pitch = 0;

        /** @brief Rotation around forward axis in radians. */
        float roll = 0;

        void setValue(const Value_type value) noexcept {
            yaw = value[0];
            pitch = value[1];
            roll = value[2];
        }

    };

}
