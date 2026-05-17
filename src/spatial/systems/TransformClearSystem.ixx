/**
 * @file TransformClearSystem.ixx
 * @brief System for resetting dirty flags on TransformComponents.
 */
module;



export module helios.engine.spatial.systems.TransformClearSystem;

import helios.engine.runtime.world.GameWorld;

import helios.engine.runtime.world.UpdateContext;

import helios.engine.spatial.components.ComposeTransformComponent;

import helios.engine.spatial.components.ScaleStateComponent;

import helios.ecs.components.Active;

import helios.engine.runtime.world.tags.SystemRole;

export namespace helios::engine::spatial::systems {

    /**
     * @brief System that clears the dirty state of TransformComponents and ScaleComponents
     * at the end of a frame.
     *
     * @details This system runs in the post-update phase. It iterates over all
     * TransformComponents and resets their dirty flag if it was set. This ensures that
     * changes are only processed once per frame by other systems (like SceneSyncSystem).
     */
    template<typename THandle>
    class TransformClearSystem {



    public:

        using EngineRoleTag = helios::engine::runtime::world::tags::SystemRole;

        /**
         * @brief Updates the system, clearing dirty flags of Transform- and ScaleStateComponent.
         *
         * @param updateContext The update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept {

            for (auto [entity, tc, active] : updateContext.view<
                THandle,
                helios::engine::spatial::components::ComposeTransformComponent<THandle>,
                helios::ecs::components::Active<THandle>
                >().whereEnabled()) {
                tc->clearDirty();
            }

            for (auto [entity, sc, active] : updateContext.view<
                THandle,
                helios::engine::spatial::components::ScaleStateComponent<THandle>,
                helios::ecs::components::Active<THandle>
            >().whereEnabled()) {
                sc->clearDirty();
            }

        }

    };

}