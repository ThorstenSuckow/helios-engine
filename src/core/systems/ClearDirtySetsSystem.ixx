/**
* @file ClearDirtySetsSystem.ixx
 * @brief System template that clears dirty sets.
 */
module;

export module helios.engine.core.systems.ClearDirtySetsSystem;


import helios.engine.runtime.gameloop.types;


import helios.ecs.component;
import helios.ecs.common.concepts;
import helios.ecs.EcsWorld;

using namespace helios::ecs::common::concepts::traits;
using namespace helios::ecs::components;

export namespace helios::engine::core::systems {

    /**
     * @brief Generic ECS system that clears dirty sets.
     *
     * @tparam TMemberHandle Member/registry handle type used to access ECS components.
     * @tparam TComponents Components which dirty set should be cleared.
     */
    template<typename TMemberHandle,
             typename ... TComponents>
    class ClearDirtySetsSystem {


    public:

        /**
         * @brief Executes one dirty-clear pass for all configured component specifications.access.
         */
        void update(ecs::EcsWorld& ecsWorld) noexcept {
            ecsWorld.clearDirtySets<TMemberHandle, TComponents...>();
        }
    };
}