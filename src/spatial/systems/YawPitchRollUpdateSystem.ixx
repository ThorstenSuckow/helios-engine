/**
 * @file YawPitchRollUpdateSystem.ixx
 * @brief System that converts yaw/pitch/roll angles to local rotation quaternions.
 */
module;

#include <cmath>
#include <numbers>

export module helios.engine.spatial.systems.YawPitchRollUpdateSystem;

import helios.engine.runtime.world.tags.SystemRole;

import helios.engine.runtime.world.UpdateContext;

import helios.ecs.components;
import helios.engine.spatial.components;


import helios.math;
import helios.engine.core.types.ComponentTypeTags;

using namespace helios::engine::core::types;
using namespace helios::ecs::components;
using namespace helios::engine::spatial::components;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::world::tags;
export namespace helios::engine::scene::systems {

    /**
     * @brief Updates `Rotation3DComponent<..., Local>` from `YawPitchRollComponent`.
     *
     * @tparam TMemberHandle ECS member handle type used by queried components.
     */
    template<typename TMemberHandle>
    class YawPitchRollUpdateSystem {

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

        /**
         * @brief Runtime role tag used for system registration.
         */
        using EngineRoleTag = TypedSystemRole;

        /**
         * @brief Executes one update pass for active entities.
         *
         * @details Reads yaw/pitch/roll angles, wraps them to `[-pi, +pi]`,
         * builds axis-angle quaternions and writes the composed local rotation.
         *
         * @param updateContext Frame-local update context with ECS access.
         */
        void update(UpdateContext& updateContext) noexcept {


            for (auto [entity, yawPitchRoll, localRotation] : updateContext.view<
                TMemberHandle,
                YawPitchRollComponent<TMemberHandle>,
                Rotation3DComponent<TMemberHandle, Local>
            >().withActive()
               .whereAllEnabled()
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

                localRotation->setValue(qYaw * qPitch * qRoll);
                entity.template markDirty<Rotation3DComponent<TMemberHandle, Local>>();
            }

        }


    };
}
