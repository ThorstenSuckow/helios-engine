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


            for (auto [entity, p3c, u3c, tp3c, pcc, active] : updateContext.view<
                TMemberHandle,
                Position3DComponent<TMemberHandle>,
                UpVector3DComponent<TMemberHandle>,
                TargetPosition3DComponent<TMemberHandle>,
                PerspectiveCameraComponent<TMemberHandle>,
                Active<TMemberHandle>
            >().whereEnabled()) {

                auto& pmc = entity.template getOrAdd<ProjectionMatrixComponent<TMemberHandle>>();
                auto& vmc = entity.template getOrAdd<ViewMatrixComponent<TMemberHandle>>();

                if (vmc.isDirty()) {
                    vmc.setValue(helios::math::lookAt(
                        p3c->value(),
                        tp3c->value(),
                        u3c->value()
                    ));
                }


                if (pmc.isDirty()) {
                    pmc.setValue(helios::math::perspective(
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