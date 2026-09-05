/**
 * @file PerspectiveCameraUpdateSystem.ixx
 * @brief System that updates view and projection matrices for active perspective cameras.
 */
module;

export module helios.engine.scene.systems.PerspectiveCameraUpdateSystem;



import helios.engine.runtime.gameloop.types;
import helios.ecs.entity.EntityWorld;


import helios.ecs.component;
import helios.engine.scene.components;
import helios.engine.spatial.components;

import helios.ecs.entity.EntityAccessSet;
import helios.ecs.entity.query.Query;

import helios.math;
import helios.engine.core.types;

using namespace helios::engine::core::types;
using namespace helios::ecs::components;
using namespace helios::engine::scene::components;
using namespace helios::engine::spatial::components;


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

        using EntityWorld = ecs::entity::EntityWorld;

        template<typename TRead, typename TWrite, typename TFilter = ecs::entity::query::Filter<ecs::entity::query::AnyDirty<>>>
        using Query = ecs::entity::query::Query<TRead, TWrite, TFilter>;

        template<typename ... TReads>
        using Read = ecs::entity::ReadSet<TReads...>;

        template<typename ... TWrites>
        using Write = ecs::entity::WriteSet<TWrites...>;

        public:

        using HandleType = TMemberHandle;


        /**
         * @brief Executes the camera update pass for all active camera entities.
         *
         * @param worldTransformQuery Query for world transform -> view matrix updates.
         * @param perspectiveQuery Query for perspective settings -> projection matrix updates.
         */
        void update(
            Query<
                Read<TransformComponent<TMemberHandle, World>,
                    ViewMatrixComponent<TMemberHandle>
                >,
                Write<ViewMatrixComponent<TMemberHandle>>,
                ecs::entity::query::Filter<
                    ecs::entity::query::IsActive,
                    ecs::entity::query::AnyDirty<
                        TransformComponent<TMemberHandle, World>,
                        Active<TMemberHandle>
                    >
                >
            > worldTransformQuery,
            Query<
                Read<PerspectiveCameraComponent<TMemberHandle>,
                    ProjectionMatrixComponent<TMemberHandle>
                >,
                Write<ProjectionMatrixComponent<TMemberHandle>>,
                ecs::entity::query::Filter<
                    ecs::entity::query::IsActive,
                    ecs::entity::query::AnyDirty<
                        PerspectiveCameraComponent<TMemberHandle>,
                        Active<TMemberHandle>
                    >
                >
            > perspectiveQuery
        ) noexcept {

            for (auto [entity, tcw, vmc] : worldTransformQuery) {

                const auto mat = tcw->value();

                const auto eye = tcw->value().translation();
                const auto center = eye + mat.column(2).toVec3().normalize();
                const auto up =  mat.column(1).toVec3().normalize();

                entity.setTrackedValue(vmc, helios::math::lookAt(eye, center, up));
            }

            for (auto [entity, pcc, pmc] : perspectiveQuery) {

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