/**
 * @file ClearAllDirtySetsSystem.ixx
 * @brief System template that clears dirty sets.
 */
module;

export module helios.engine.core.systems.ClearAllDirtySetsSystem;

import helios.engine.runtime.gameloop.types;


import helios.ecs.component;
import helios.ecs.entity.EntityWorld;
import helios.ecs.common.concepts;



using namespace helios::ecs::common::concepts::traits;
using namespace helios::ecs::components;


export namespace helios::engine::core::systems {

    /**
     * @brief Generic ECS system that clears engine wide dirty sets.
     */
    class ClearAllDirtySetsSystem {

        using UpdateContext = engine::runtime::gameloop::types::UpdateContext;

    public:


        /**
         * @brief Executes one dirty-clear pass for all configured component specifications.
         *
         * @param ecsWorld Frame-local update context with ECS access.
         * @return true if successful, false otherwise.
         */
        void update(ecs::entity::EntityWorld& ecsWorld) noexcept {

            ecsWorld.clearDirtySets();
        }
    };
}