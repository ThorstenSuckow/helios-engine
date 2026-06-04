/**
 * @file SceneMemberVisibilityRegistryClearSystem.ixx
 * @brief System that clears the SceneMemberVisibilityRegistry once per update.
 */
module;

#include <concepts>
#include <cassert>
#include "helios-engine-config.h"

export module helios.engine.scene.systems.SceneMemberVisibilityRegistryClearSystem;

import helios.engine.rendering.viewport.concepts.IsViewportHandle;

import helios.engine.scene.SceneMemberVisibilityRegistry;
import helios.engine.scene.components;

import helios.engine.rendering.common.components;
import helios.engine.rendering.common.commands;

import helios.engine.spatial.components;

import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.tags.SystemRole;

import helios.ecs.components.Active;


import helios.engine.util.log;

import helios.math;

using namespace helios::engine::scene;
using namespace helios::ecs::components;

#define HELIOS_LOG_SCOPE "helios::engine::scene::systems::SceneMemberVisibilityRegistryClearSystem"
export namespace helios::engine::scene::systems {

    /**
     * @brief ECS system that clears the visibility registry at the start of a frame.
     * @tparam TMemberHandle Handle type used for registered scene members.
     */
    template<typename TMemberHandle>
    class SceneMemberVisibilityRegistryClearSystem {

        SceneMemberVisibilityRegistry<TMemberHandle>& registry_;

    public:

        using EngineRoleTag = helios::engine::runtime::world::tags::SystemRole;

        /**
         * @brief Creates the system with an existing visibility registry.
         * @param registry Reference to the registry that will be cleared.
         */
        explicit SceneMemberVisibilityRegistryClearSystem(SceneMemberVisibilityRegistry<TMemberHandle>& registry)
        : registry_(registry) {}

        /**
         * @brief Clears the registry for the current update cycle.
         * @param updateContext Runtime context of the engine update.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept {
            registry_.clear();
        }

    };

}