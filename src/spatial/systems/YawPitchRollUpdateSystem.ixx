/**
 * @file YawPitchRollUpdateSystem.ixx
 * @brief System that converts yaw/pitch/roll angles to local rotation quaternions.
 */
module;

#include <cmath>
#include <numbers>

export module helios.engine.spatial.systems.YawPitchRollUpdateSystem;



import helios.engine.runtime.gameloop.types;
import helios.ecs.EcsWorld;


import helios.ecs.component;
import helios.engine.spatial.components;


import helios.math;
import helios.engine.core.types;

using namespace helios::engine::core::types;
using namespace helios::ecs::components;
using namespace helios::engine::spatial::components;


export namespace helios::engine::scene::systems {

    /**
     * @brief Updates `Rotation3DComponent<..., Local>` from `YawPitchRollComponent`.
     *
     * @tparam TMemberHandle ECS member handle type used by queried components.
     */
    template<typename TMemberHandle>
    class YawPitchRollUpdateSystem {

        using EcsWorld = ecs::EcsWorld;

        /**
         * @brief Wraps an angle to the interval `[-pi, +pi]`.
         *
         * @param a Input angle in radians.
         * @return Wrapped angle in radians.
         */
        [[nodiscard]] float wrapRad(float a) const noexcept {
            constexpr float pi = std::numbers::pi_v<float>;
            constexpr float twoPi = 2.0f * pi;
            return std::fmod(std::fmod(a + pi, twoPi) + twoPi, twoPi) - pi;
        }

    public:

        using Handle_type = TMemberHandle;


        /**
         * @brief Executes one update pass for active entities.
         *
         * @details Reads yaw/pitch/roll angles, wraps them to `[-pi, +pi]`,
         * builds axis-angle quaternions and writes the composed local rotation.
         *
         * @param ecsWorld Frame-local ECS world.
         * @return true if the update was successful, false otherwise.
         */
        void update(EcsWorld& ecsWorld) noexcept {

            for (auto [entity, yawPitchRoll, localRotation] : ecsWorld.view<
                TMemberHandle,
                YawPitchRollComponent<TMemberHandle>,
                Rotation3DComponent<TMemberHandle, Local>
            >().withActive()
               .template whereAnyDirty<
               YawPitchRollComponent<TMemberHandle>,
               Active<TMemberHandle>
            >()) {
                constexpr auto x = helios::math::X_AXISf;
                constexpr auto y = helios::math::Y_AXISf;
                constexpr auto z = helios::math::Z_AXISf;

                const auto yaw = wrapRad(yawPitchRoll->yaw);
                const auto pitch = wrapRad(yawPitchRoll->pitch);
                const auto roll = wrapRad(yawPitchRoll->roll);

                const auto qYaw = helios::math::quatf::fromAxisAngle(y, yaw);
                const auto qPitch = helios::math::quatf::fromAxisAngle(x, pitch);
                const auto qRoll = helios::math::quatf::fromAxisAngle(z, roll);

                entity.setTrackedValue(localRotation, qYaw * qPitch * qRoll);
            }

        }


    };
}
