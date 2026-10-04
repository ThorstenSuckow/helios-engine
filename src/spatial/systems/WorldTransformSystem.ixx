/**
 * @file WorldTransformSystem.ixx
 * @brief System that propagates local position changes to world transforms.
 */
module;

export module helios.engine.spatial.systems.WorldTransformSystem;



import helios.engine.runtime.gameloop.types;
import helios.ecs.entity.EntityWorld;

import helios.ecs.component;
import helios.engine.spatial.components;

import helios.ecs.entity.QueryAccessSet;
import helios.ecs.entity.query.Query;


import helios.math;
import helios.engine.core.types;

using namespace helios::engine::core::types;
using namespace helios::ecs::components;
using namespace helios::engine::spatial::components;


export namespace helios::engine::scene::systems {

    /**
     * @brief Updates world-space transforms from local position components.
     *
     * @tparam TMemberHandle ECS member handle type used by queried components.
     */
    template<typename TMemberHandle>
    class WorldTransformSystem {

        using EntityWorld = ecs::entity::EntityWorld;

        template<typename TRead, typename TWrite, typename TFilter = ecs::entity::query::Filter<ecs::entity::query::AnyDirty<>>>
        using Query = ecs::entity::query::Query<TMemberHandle, TRead, TWrite, TFilter>;

        template<typename ... TReads>
        using Read = ecs::entity::ReadSet<TReads...>;

        template<typename ... TWrites>
        using Write = ecs::entity::WriteSet<TWrites...>;

    public:

        using HandleType = TMemberHandle;


        /**
         * @brief Executes one update pass over active transform tuples.
         *
         * @details For each active entity, the world transform translation is updated
         * only when the local position component is marked dirty.
         *
         * @param query Frame-local query over required transform components.
         */
        void update(Query<
                Read<Position3DComponent<Local>,
                    Rotation3DComponent<Local>,
                    TransformComponent<World>
                >, Write<
                    TransformComponent<World>
                >,
                ecs::entity::query::Filter<
                    ecs::entity::query::IsActive,
                    ecs::entity::query::AnyDirty<
                        Active,
                        Position3DComponent<Local>,
                        Rotation3DComponent<Local>
                    >
                >
            > query) noexcept {

            for (auto [
                entity,
                localPosition,
                localRotation,
                worldTransform
                ] : query) {

                entity.template track<TransformComponent<World>>()
                    ->setValue(
                    localRotation->value().rotationMatrix().withTranslation(localPosition->value())
                );

            }
        }


    };
}
