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

import helios.ecs.entity.EntityAccessSet;
import helios.ecs.entity.Query;


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

        template<typename TRead, typename TWrite, typename TFilter = ecs::entity::Filter<ecs::entity::AnyDirty<>>>
        using Query = ecs::entity::Query<TRead, TWrite, TFilter>;

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
                Read<Position3DComponent<TMemberHandle, Local>,
                    Rotation3DComponent<TMemberHandle, Local>,
                    TransformComponent<TMemberHandle, World>
                >, Write<
                    TransformComponent<TMemberHandle, World>
                >,
                ecs::entity::Filter<
                    ecs::entity::IsActive,
                    ecs::entity::AnyDirty<
                        Active<TMemberHandle>,
                        Position3DComponent<TMemberHandle, Local>,
                        Rotation3DComponent<TMemberHandle, Local>
                    >
                >
            > query) noexcept {

            for (auto [
                entity,
                localPosition,
                localRotation,
                worldTransform
                ] : query) {
                entity.setTrackedValue(
                    worldTransform,
                    localRotation->value().rotationMatrix().withTranslation(localPosition->value())
                );

            }
        }


    };
}
