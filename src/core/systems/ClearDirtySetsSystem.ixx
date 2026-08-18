/**
* @file ClearDirtySetsSystem.ixx
 * @brief System template that clears dirty sets.
 */
module;

export module helios.engine.core.systems.ClearDirtySetsSystem;

import helios.ecs.system.tags;
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
     * @brief Generic ECS system that clears dirty sets.
     *
     * @tparam TMemberHandle Member/registry handle type used to access ECS components.
     * @tparam TUpdateContextType Type of the UpdateContext to use.
     * @tparam TComponents Components which dirty set should be cleared.
     */
    template<typename TMemberHandle,
             typename TUpdateContextType = types::SystemUpdateContext,
             typename ... TComponents>
    requires runtime::concepts::ProvidesUpdateContext<TUpdateContextType, UpdateContext>
    class ClearDirtySetsSystem {

    public:

        /**
         * @brief Runtime role tag used for system registration.
         */
        using EcsRoleTag = ecs::system::tags::TypedSystemRole;
        using UpdateContextType = TUpdateContextType;

        /**
         * @brief Executes one dirty-clear pass for all configured component specifications.
         *
         * @param updateCtx Frame-local update context with ECS access.
         * @return true if the update was successful, false otherwise.
         */
        bool update(TUpdateContextType& updateCtx) noexcept {
            auto& updateContext = updateCtx.updateContext();
            updateContext.template clearDirtySets<TMemberHandle, TComponents...>();
            return true;
        }
    };
}