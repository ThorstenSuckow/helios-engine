/**
 * @file ClearAllDirtySetsSystem.ixx
 * @brief System template that clears dirty sets.
 */
module;

export module helios.engine.core.systems.ClearAllDirtySetsSystem;

import helios.engine.runtime.world.UpdateContext;

import helios.engine.runtime.world.types;
import helios.engine.runtime.concepts;

import helios.ecs.component;

import helios.ecs.common.concepts;


using namespace helios::ecs::common::concepts::traits;
using namespace helios::ecs::components;
using namespace helios::engine::runtime::world;

export namespace helios::engine::core::systems {

    /**
     * @brief Generic ECS system that clears engine wide dirty sets.
     */
    class ClearAllDirtySetsSystem {

    public:


        /**
         * @brief Executes one dirty-clear pass for all configured component specifications.
         *
         * @param updateContext Frame-local update context with ECS access.
         * @return true if successful, false otherwise.
         */
        void update(UpdateContext& updateContext) noexcept {

            updateContext.clearDirtySets();
        }
    };
}