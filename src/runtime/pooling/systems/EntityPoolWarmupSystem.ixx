/**
 * @file EntityPoolWarmupSystem.ixx
 * @brief System that translates PrefabRequestComponent into PrefabComponentPoolCommand.
 */
module;


export module helios.engine.runtime.pooling.systems:EntityPoolWarmupSystem;

import helios.engine.runtime.world.tags.SystemRole;

import helios.engine.runtime.pooling.commands;
import helios.engine.runtime.pooling.types;
import helios.engine.runtime.pooling.components;

import helios.engine.runtime.world.UpdateContext;

export namespace helios::engine::runtime::pooling::systems {

    /**
     * @brief System for creating PrefabEntityPoolCommand from PrefabEntityPoolRequestComponent.
     *
     * @tparam TMemberHandle The type of the member handle.
     *
     * @tparam TCommandBuffer The target command buffer.
     */
    template<typename TMemberHandle, typename TCommandBuffer>
    class EntityPoolWarmupSystem {


    public:

        /**
         * @brief Command buffer type this system emits into.
         */
        using CommandBuffer_type = TCommandBuffer;

        /**
         * @brief Marks this system as a typed system role.
         */
        using EngineRoleTag = world::tags::TypedSystemRole;


        /**
         * @brief Picks up any PrefabEntityPoolRequestComponent and creates a PrefabEntityPoolCommand.
         *
         * @details Subsequent managers are responsible for removing the component to
         * prevent multiple prefabs from being registered.
         *
         * @param updateContext The current update context providing entity views.
         * @param cmdBuffer The command buffer receiving the emitted pool commands.
         */
        void update(world::UpdateContext& updateContext, TCommandBuffer& cmdBuffer) {

            for (auto [entity, requestComponent, keyComponent] : updateContext.view<
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