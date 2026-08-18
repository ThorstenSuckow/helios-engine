/**
 * @file EntityPoolWarmupSystem.ixx
 * @brief System that translates PrefabRequestComponent into PrefabComponentPoolCommand.
 */
module;


export module helios.engine.runtime.pooling.systems:EntityPoolWarmupSystem;

import helios.ecs.system.tags;

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
     * @tparam TCommandBuffer The target command buffer.
     * @tparam TUpdateContextType Type of the UpdateContext to use.
     */
    template<typename TMemberHandle,
             typename TCommandBuffer,
             typename TUpdateContextType = world::types::SystemUpdateContext>
    requires runtime::concepts::ProvidesUpdateContext<TUpdateContextType, world::UpdateContext>
    class EntityPoolWarmupSystem {


    public:

        /**
         * @brief Command buffer type this system emits into.
         */
        using CommandBuffer_type = TCommandBuffer;
        using UpdateContextType = TUpdateContextType;


        /**
         * @brief Marks this system as a typed system role.
         */
        using EcsRoleTag = ecs::system::tags::TypedSystemRole;


        /**
         * @brief Picks up any PrefabEntityPoolRequestComponent and creates a PrefabEntityPoolCommand.
         *
         * @details Subsequent managers are responsible for removing the component to
         * prevent multiple prefabs from being registered.
         *
         * @param updateCtx The current update context providing entity views.
         * @param cmdBuffer The command buffer receiving the emitted pool commands.
         */
        bool update(TUpdateContextType& updateCtx, TCommandBuffer& cmdBuffer) {

            auto& updateContext = updateCtx.updateContext();

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
            return true;
        }

    };





}