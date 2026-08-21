/**
 * @file WorldTransformSystem.ixx
 * @brief System that propagates local position changes to world transforms.
 */
module;

export module helios.engine.spatial.systems.WorldTransformSystem;

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
     * @brief Updates world-space transforms from local position components.
     *
     * @tparam TMemberHandle ECS member handle type used by queried components.
     */
    template<typename TMemberHandle>
    class WorldTransformSystem {

    public:

        using Handle_type = TMemberHandle;

        /**
         * @brief Runtime role tag used for system registration.
         */
        using EcsRoleTag = ecs::system::tags::TypedSystemRole;

        /**
         * @brief Executes one update pass over active transform tuples.
         *
         * @details For each active entity, the world transform translation is updated
         * only when the local position component is marked dirty.
         *
         * @param updateCtx Frame-local update context with ECS access.
         */
        template<typename TUpdateContextType>
        requires runtime::concepts::ProvidesUpdateContext<TUpdateContextType, UpdateContext>
        void update(TUpdateContextType& updateCtx) noexcept {

            auto& updateContext = updateCtx.updateContext();

            for (auto [
                entity,
                localPosition,
                localRotation,
                worldTransform
                ] : updateContext.template view<
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
