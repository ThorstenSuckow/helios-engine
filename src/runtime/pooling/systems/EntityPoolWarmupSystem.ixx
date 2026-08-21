/**
 * @file EntityPoolWarmupSystem.ixx
 * @brief System that translates PrefabRequestComponent into PrefabComponentPoolCommand.
 */
module;


export module helios.engine.runtime.pooling.systems:EntityPoolWarmupSystem;

import helios.ecs.system.tags;
import helios.ecs.common.concepts;
import helios.ecs.command.concepts;
import helios.ecs.command.types;

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

        /**
         * @brief Marks this system as a typed system role.
         */
        using EcsRoleTag = ecs::system::tags::TypedSystemRole;
        using CommandTypes = ecs::command::types::CommandTypeList<commands::PrefabEntityPoolCommand<TMemberHandle>>;


        /**
         * @brief Picks up any PrefabEntityPoolRequestComponent and creates a PrefabEntityPoolCommand.
         *
         * @details Subsequent managers are responsible for removing the component to
         * prevent multiple prefabs from being registered.
         *
         * @param updateCtx The current update context providing entity views.
         * @param cmdBuffer The command buffer receiving the emitted pool commands.
         */
        template<typename TUpdateContextType, typename TCommandBuffer>
        requires runtime::concepts::ProvidesUpdateContext<TUpdateContextType, world::UpdateContext> &&
                 ecs::command::concepts::IsCommandBufferLike<TCommandBuffer>
        void update(TUpdateContextType& updateCtx, TCommandBuffer& cmdBuffer) noexcept {

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
        }

    };





}