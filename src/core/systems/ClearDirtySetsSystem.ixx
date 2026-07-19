/**
* @file ClearDirtySetsSystem.ixx
 * @brief System template that clears dirty sets.
 */
module;

export module helios.engine.core.systems.ClearDirtySetsSystem;

import helios.engine.runtime.world.tags.SystemRole;
import helios.engine.runtime.world.UpdateContext;

import helios.ecs.components;

import helios.ecs.concepts.Traits;


using namespace helios::ecs::concepts::traits;
using namespace helios::ecs::components;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::world::tags;
export namespace helios::engine::core::systems {

    /**
     * @brief Generic ECS system that clears dirty sets.
     *
     * @tparam TMemberHandle Member/registry handle type used to access ECS components.
     * @tparam TComponents Components which dirty set should be cleared.
     */
    template<typename TMemberHandle, typename ... TComponents>
    class ClearDirtySetsSystem {

    public:

        /**
         * @brief Runtime role tag used for system registration.
         */
        using EngineRoleTag = TypedSystemRole;

        /**
         * @brief Executes one dirty-clear pass for all configured component specifications.
         *
         * @param updateContext Frame-local update context with ECS access.
         */
        void update(UpdateContext& updateContext) noexcept {
            updateContext.clearDirtySets<TMemberHandle, TComponents...>();
        }
    };
}