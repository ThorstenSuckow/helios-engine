/**
 * @file WorldBoundsUpdateSystem.ixx
 * @brief System that updates world-space bounds from local bounds and world transform.
 */
module;


export module helios.engine.spatial.systems.WorldBoundsUpdateSystem;

import helios.ecs.system.tags;

import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.types;
import helios.engine.runtime.concepts;

import helios.ecs.component;
import helios.engine.spatial.components;


import helios.math;
import helios.engine.core.types;

using namespace helios::engine::core::types;
using namespace helios::ecs::components;
using namespace helios::engine::spatial::components;
using namespace helios::engine::runtime::world;

export namespace helios::engine::scene::systems {

    /**
     * @brief Updates world-space bounds for active entities.
     *
     * @tparam TMemberHandle ECS member handle type used by queried components.
     */
    template<typename TMemberHandle>
    class WorldBoundsUpdateSystem {

    public:

        using Handle_type = TMemberHandle;

        /**
         * @brief Runtime role tag used for system registration.
         */
        using EcsRoleTag = ecs::system::tags::TypedSystemRole;

        /**
         * @brief Executes one update pass over active bounds tuples.
         *
         * @details For each active entity, world-space bounds are recomputed only
         * when the world transform component is marked dirty.
         *
         * @param updateCtx Frame-local update context with ECS access.
         */
        template<typename TUpdateContextType>
        requires runtime::concepts::ProvidesUpdateContext<TUpdateContextType, UpdateContext>
        void update(TUpdateContextType& updateCtx) noexcept {

            auto& updateContext = updateCtx.updateContext();

            for (auto [entity, boundsLocal, boundsWorld, worldTransform] : updateContext.template view<
                TMemberHandle,
                BoundsComponent<TMemberHandle, Local>,
                BoundsComponent<TMemberHandle, World>,
                TransformComponent<TMemberHandle, World>
            >().withActive()
               .template whereAnyDirty<
                    BoundsComponent<TMemberHandle, Local>,
                    TransformComponent<TMemberHandle, World>,
                    Active<TMemberHandle>
                >()
               ) {

                entity.setTrackedValue(boundsWorld, boundsLocal->value().applyTransform(worldTransform->value()));

            }
        }


    };
}
