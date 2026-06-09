/**
 * @file PerspectiveCameraUpdateSystem.ixx
 * @brief System that updates view and projection matrices for active perspective cameras.
 */
module;

export module helios.engine.scene.systems.PerspectiveCameraUpdateSystem;

import helios.engine.runtime.world.tags.SystemRole;

import helios.engine.runtime.world.UpdateContext;

import helios.ecs.components;
import helios.engine.scene.components;
import helios.engine.spatial.components;

import helios.math;
import helios.engine.core.types.ComponentTypeTags;

using namespace helios::engine::core::types;
using namespace helios::ecs::components;
using namespace helios::engine::scene::components;
using namespace helios::engine::spatial::components;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::world::tags;
export namespace helios::engine::scene::systems {

    /**
     * @brief Updates camera transforms for active perspective cameras.
     *
     * @details Recomputes LookAt and Perspective matrices only when the respective
     * target components are marked as dirty.
     *
     * @tparam TMemberHandle Camera entity handle type.
     */
    template<typename TMemberHandle>
    class PerspectiveCameraUpdateSystem {

        public:

        /**
         * @brief Role tag used for runtime registration as a system.
         */
        using EngineRoleTag = SystemRole;

        /**
         * @brief Executes the camera update pass for all active camera entities.
         *
         * @param updateContext Frame-local update context with ECS access.
         */
        void update(UpdateContext& updateContext) noexcept {


            for (auto [entity, tcw, pcc, pmc, vmc, active] : updateContext.view<
                TMemberHandle,
                TransformComponent<TMemberHandle, World>,
                PerspectiveCameraComponent<TMemberHandle>,
                ProjectionMatrixComponent<TMemberHandle>,
                ViewMatrixComponent<TMemberHandle>,
                Active<TMemberHandle>
            >().whereEnabled()) {

                if (tcw->isDirty()) {

                    const auto mat = tcw->value();

                    const auto eye = tcw->value().translation();
                    const auto center = eye + mat.column(2).toVec3().normalize();
                    const auto up =  mat.column(1).toVec3().normalize();

                    vmc->setValue(helios::math::lookAt(eye, center, up));
                }


                if (pcc->isDirty()) {
                    pmc->setValue(helios::math::perspective(
                        pcc->fovY(),
                        pcc->aspectRatio(),
                        pcc->zNear(),
                        pcc->zFar()
                    ));
               }

            }

        }


    };


}