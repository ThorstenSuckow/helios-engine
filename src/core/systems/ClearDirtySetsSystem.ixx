/**
* @file ClearDirtySetsSystem.ixx
 * @brief System template that clears dirty sets.
 */
module;

export module helios.engine.core.systems.ClearDirtySetsSystem;


import helios.engine.runtime.gameloop.types;


import helios.ecs.component;

import helios.ecs.common.concepts;


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

        using UpdateContext = engine::runtime::gameloop::types::UpdateContext;

    public:

        /**
         * @brief Executes one dirty-clear pass for all configured component specifications.
         *
         * @param updateContext Frame-local update context with ECS access.
         * @return true if the update was successful, false otherwise.
         */
        void update(UpdateContext& updateContext) noexcept {
            updateContext.template clearDirtySets<TMemberHandle, TComponents...>();
        }
    };
}