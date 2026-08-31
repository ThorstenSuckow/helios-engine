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

    public:

        using Handle_type = TMemberHandle;


        /**
         * @brief Executes one update pass over active transform tuples.
         *
         * @details For each active entity, the world transform translation is updated
         * only when the local position component is marked dirty.
         *
         * @param ecsWorld Frame-local ECS world.
         */
        void update(EntityWorld& ecsWorld) noexcept {

            for (auto [
                entity,
                localPosition,
                localRotation,
                worldTransform
                ] : ecsWorld.view<
                TMemberHandle,
                Position3DComponent<TMemberHandle, Local>,
                Rotation3DComponent<TMemberHandle, Local>,
                TransformComponent<TMemberHandle, World>
            >().withActive()
                .template whereAnyDirty<
                Active<TMemberHandle>,
                Position3DComponent<TMemberHandle, Local>,
                Rotation3DComponent<TMemberHandle, Local>
            >()) {

                entity.setTrackedValue(
                    worldTransform,
                    localRotation->value().rotationMatrix().withTranslation(localPosition->value())
                );

            }
        }


    };
}
