/**
 * @file PerspectiveCameraUpdateSystem.ixx
 * @brief System that updates view and projection matrices for active perspective cameras.
 */
module;

export module helios.engine.scene.systems.PerspectiveCameraUpdateSystem;



import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.concepts;

import helios.ecs.component;
import helios.engine.scene.components;
import helios.engine.spatial.components;

import helios.math;
import helios.engine.core.types;

using namespace helios::engine::core::types;
using namespace helios::ecs::components;
using namespace helios::engine::scene::components;
using namespace helios::engine::spatial::components;
using namespace helios::engine::runtime::world;

export namespace helios::engine::scene::systems {

    /**
     * @brief Updates camera transforms for active perspective cameras.
     *
     * @details Recomputes LookAt and Perspective matrices only when the respective
     * target components are marked as dirty.
     *
     * @tparam TMemberHandle Camera entity handle type.
     *
     * @note Although this system could be separated into to concurrently running systems,
     * the regular use case for this system is that it only has to consider few camera entities per frames.
     */
    template<typename TMemberHandle>
    class PerspectiveCameraUpdateSystem {

        public:

        using Handle_type = TMemberHandle;


        /**
         * @brief Executes the camera update pass for all active camera entities.
         *
         * @param updateContext Frame-local update context with ECS access.
         */
        void update(UpdateContext& updateContext) noexcept {

            for (auto [entity, tcw, vmc] : updateContext.template view<
                TMemberHandle,
                TransformComponent<TMemberHandle, World>,
                ViewMatrixComponent<TMemberHandle>
            >().withActive()

               .template whereAnyDirty<
                    TransformComponent<TMemberHandle, World>,
                    Active<TMemberHandle>
                >()) {

                const auto mat = tcw->value();

                const auto eye = tcw->value().translation();
                const auto center = eye + mat.column(2).toVec3().normalize();
                const auto up =  mat.column(1).toVec3().normalize();

                entity.setTrackedValue(vmc, helios::math::lookAt(eye, center, up));
            }

            for (auto [entity, pcc, pmc] : updateContext.template view<
                TMemberHandle,
                PerspectiveCameraComponent<TMemberHandle>,
                ProjectionMatrixComponent<TMemberHandle>
            >().withActive()
               .template whereAnyDirty<
                    PerspectiveCameraComponent<TMemberHandle>,
                    Active<TMemberHandle>
                >()) {

                entity.setTrackedValue(pmc, helios::math::perspective(
                    pcc->fovY(),
                    pcc->aspectRatio(),
                    pcc->zNear(),
                    pcc->zFar()
                ));
            }

        }


    };


}