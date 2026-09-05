/**
 * @file WorldBoundsUpdateSystem.ixx
 * @brief System that updates world-space bounds from local bounds and world transform.
 */
module;


export module helios.engine.spatial.systems.WorldBoundsUpdateSystem;



import helios.engine.runtime.gameloop.types;
import helios.ecs.entity.EntityWorld;

import helios.ecs.component;
import helios.engine.spatial.components;


import helios.ecs.entity.EntityAccessSet;
import helios.ecs.entity.query.Query;

import helios.math;
import helios.engine.core.types;

using namespace helios::engine::core::types;
using namespace helios::ecs::components;
using namespace helios::engine::spatial::components;


export namespace helios::engine::scene::systems {

    /**
     * @brief Updates world-space bounds for active entities.
     *
     * @tparam TMemberHandle ECS member handle type used by queried components.
     */
    template<typename TMemberHandle>
    class WorldBoundsUpdateSystem {

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
         * @brief Executes one update pass over active bounds tuples.
         *
         * @details For each active entity, world-space bounds are recomputed only
         * when the world transform component is marked dirty.
         *
         * @param ecsWorld Frame-local ECS world.
         */
        void update(Query<
                Read<BoundsComponent<TMemberHandle, Local>,
                    BoundsComponent<TMemberHandle, World>,
                    TransformComponent<TMemberHandle, World>
                >, Write<
                    BoundsComponent<TMemberHandle, World>
                >,
                ecs::entity::query::Filter<
                    ecs::entity::query::IsActive,
                    ecs::entity::query::AnyDirty<
                        BoundsComponent<TMemberHandle, Local>,
                        TransformComponent<TMemberHandle, World>,
                        Active<TMemberHandle>
                    >
                >
            > query) noexcept {

            for (auto [
                    entity,
                    boundsLocal,
                    boundsWorld,
                    worldTransform
                ] : query) {
                entity.setTrackedValue(boundsWorld, boundsLocal->value().applyTransform(worldTransform->value()));
            }
        }


    };
}
