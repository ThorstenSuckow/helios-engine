/**
 * @file ClearDirtySystem.ixx
 * @brief System template that clears dirty flags on active dirty-tracker components.
 */
module;

export module helios.engine.core.systems.ClearDirtySystem;

import helios.engine.runtime.world.tags.SystemRole;
import helios.engine.runtime.world.UpdateContext;

import helios.ecs.components;

import helios.engine.core.concepts.IsDirtyTrackerLike;

using namespace helios::ecs::components;
using namespace helios::engine::runtime::world;
using namespace helios::engine::core::concepts;
using namespace helios::engine::runtime::world::tags;
export namespace helios::engine::core::systems {

    /**
     * @brief Generic ECS system that resets dirty state for configured component types.
     *
     * @tparam TMemberHandle Member/registry handle type used to access ECS components.
     * @tparam TComponentSpecs Component type specifications resolved via
     * `TComponentSpec::type<TMemberHandle>`.
     *
     * @details For each configured component type, the system iterates all entities with
     * `Active<TMemberHandle>` enabled and calls `clearDirty()` on the component instance.
     */
    template<typename TMemberHandle, typename ... TComponentSpecs>
    requires (IsDirtyTrackerLike<typename TComponentSpecs::template type<TMemberHandle>> && ...)
    class ClearDirtySystem {

        /**
         * @brief Clears the dirty flag for one configured component specification.
         *
         * @tparam TComponentSpec Component specification type.
         * @param updateContext Frame-local update context providing ECS view access.
         */
        template<typename TComponentSpec>
        void clearDirtyFor(UpdateContext& updateContext) {

            using Component = typename TComponentSpec::template type<TMemberHandle>;

            for (auto [entity, cmp, active] : updateContext.view<
                TMemberHandle,
                Component,
                Active<TMemberHandle>>().whereEnabled()) {
                cmp->clearDirty();
                }
        }



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
            (clearDirtyFor<TComponentSpecs>(updateContext), ...);
        }
    };
}