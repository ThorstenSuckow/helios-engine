/**
 * @file EntityPoolWarmupSystem.ixx
 * @brief System that translates PrefabRequestComponent into PrefabComponentPoolCommand.
 */
module;


export module helios.engine.runtime.pooling.systems:EntityPoolWarmupSystem;


import helios.ecs.common;
import helios.ecs.command;

import helios.engine.runtime.pooling.commands;
import helios.engine.runtime.pooling.types;
import helios.engine.runtime.pooling.components;

import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.types;
import helios.engine.runtime.concepts;

export namespace helios::engine::runtime::pooling::systems {

    /**
     * @brief System for creating PrefabEntityPoolCommand from PrefabEntityPoolRequestComponent.
     *
     * @tparam TMemberHandle The type of the member handle.
     */
    template<typename TMemberHandle>
    class EntityPoolWarmupSystem {


    public:

        using CommandBuffer = ecs::command::TypedCommandBuffer<commands::PrefabEntityPoolCommand<TMemberHandle>>;


        /**
         * @brief Picks up any PrefabEntityPoolRequestComponent and creates a PrefabEntityPoolCommand.
         *
         * @details Subsequent managers are responsible for removing the component to
         * prevent multiple prefabs from being registered.
         *
         * @param updateContext The current update context providing entity views.
         * @param cmdBuffer The command buffer receiving the emitted pool commands.
         */
        void update(world::UpdateContext& updateContext, CommandBuffer& cmdBuffer) noexcept {

            for (auto [entity, requestComponent, keyComponent] : updateContext.template view<
                TMemberHandle,
                components::PrefabEntityPoolRequestComponent<TMemberHandle>,
                components::EntityPoolKeyComponent<TMemberHandle>
            >()) {

                cmdBuffer.template add<commands::PrefabEntityPoolCommand<TMemberHandle>>(
                    keyComponent->entityPoolKey,
                    entity.handle(),
                    requestComponent->value()
                );

            }
        }

    };





}