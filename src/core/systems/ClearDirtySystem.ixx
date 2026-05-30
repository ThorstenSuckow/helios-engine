/**
 * @file ClearDirtySystem.ixx
 * @brief Generic system that clears dirty flags on configured active components.
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
     * @brief Clears dirty flags for configured component templates on active entities.
     *
     * @details Iterates active entities for each configured component type and calls
     * `clearDirty()` on matching components.
     *
     * @tparam TMemberHandle Entity handle type.
     * @tparam TComponents Component templates that satisfy `IsDirtyTrackerLike`.
     */
    template<typename TMemberHandle, template<typename> typename ... TComponents>
    requires (IsDirtyTrackerLike<TComponents<TMemberHandle>> && ...)
    class ClearDirtySystem {

        /**
         * @brief Clears the dirty flag for one component type.
         *
         * @tparam TComponent Component template to process.
         * @param updateContext Frame-local update context.
         */
        template<template<typename> typename TComponent>
        void clearDirtyFor(UpdateContext& updateContext) {
            for (auto [entity, cmp, active] : updateContext.view<
                TMemberHandle,
                TComponent<TMemberHandle>,
                Active<TMemberHandle>>().whereEnabled()) {
                cmp->clearDirty();
                }
        }

        /**
         * @brief Clears dirty flags for all configured component types.
         *
         * @tparam Args Component templates to process.
         * @param updateContext Frame-local update context.
         */
        template<template<typename> typename... Args>
        void clearDirtyForAll(UpdateContext& updateContext) {
            (clearDirtyFor<Args>(updateContext), ...);
        }


    public:

        /**
         * @brief Role tag used for runtime registration as a system.
         */
        using EngineRoleTag = SystemRole;

        /**
         * @brief Executes the dirty-clear pass for all configured component types.
         *
         * @param updateContext Frame-local update context with ECS access.
         */
        void update(UpdateContext& updateContext) noexcept {
            clearDirtyForAll<TComponents...>(updateContext);
        }
    };
}