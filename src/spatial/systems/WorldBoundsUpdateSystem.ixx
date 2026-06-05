/**
 * @file WorldBoundsUpdateSystem.ixx
 * @brief System that updates world-space bounds from local bounds and world transform.
 */
module;


export module helios.engine.spatial.systems.WorldBoundsUpdateSystem;

import helios.engine.runtime.world.tags.SystemRole;

import helios.engine.runtime.world.UpdateContext;

import helios.ecs.components;
import helios.engine.spatial.components;


import helios.math;
import helios.engine.core.types.ComponentTypeTags;

using namespace helios::engine::core::types;
using namespace helios::ecs::components;
using namespace helios::engine::spatial::components;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::world::tags;
export namespace helios::engine::scene::systems {

    /**
     * @brief Updates world-space bounds for active entities.
     *
     * @tparam TMemberHandle ECS member handle type used by queried components.
     */
    template<typename TMemberHandle>
    class WorldBoundsUpdateSystem {

    public:

        /**
         * @brief Runtime role tag used for system registration.
         */
        using EngineRoleTag = SystemRole;

        /**
         * @brief Executes one update pass over active bounds tuples.
         *
         * @details For each active entity, world-space bounds are recomputed only
         * when the world transform component is marked dirty.
         *
         * @param updateContext Frame-local update context with ECS access.
         */
        void update(UpdateContext& updateContext) noexcept {


            for (auto [entity, boundsLocal, boundsWorld, worldTransform, active] : updateContext.view<
                TMemberHandle,
                BoundsComponent<TMemberHandle, Local>,
                BoundsComponent<TMemberHandle, World>,
                TransformComponent<TMemberHandle, World>,
                Active<TMemberHandle>
            >().whereEnabled()) {

                if (worldTransform->isDirty()) {
                    boundsWorld->setValue(boundsLocal->value().applyTransform(worldTransform->value()));
                }

            }

        }


    };
}
