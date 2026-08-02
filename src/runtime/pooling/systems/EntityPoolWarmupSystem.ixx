module;


export module helios.engine.runtime.pooling.systems:EntityPoolWarmupSystem;

import helios.engine.runtime.world.tags.SystemRole;

import helios.engine.runtime.pooling.commands;
import helios.engine.runtime.pooling.types;
import helios.engine.runtime.pooling.components;

import helios.engine.runtime.world.UpdateContext;

export namespace helios::engine::runtime::pooling::systems {

    /**
     * @brief System for creating PrefabComponentPoolCommand from EntityPoolPrefabComponent.
     *
     * @tparam TMemberHandle The type of the member handle.
     *
     * @tparam TCommandBuffer The target command buffer.
     */
    template<typename TMemberHandle, typename TCommandBuffer>
    class EntityPoolWarmupSystem {


    public:

        using CommandBuffer_type = TCommandBuffer;

        using EngineRoleTag = world::tags::TypedSystemRole;


        void update(world::UpdateContext& updateContext, TCommandBuffer& cmdBuffer) {

            for (auto [entity, prefabComponent] : updateContext.view<
                TMemberHandle,
                components::EntityPoolPrefabComponent<TMemberHandle>
            >().withActive()) {

                cmdBuffer.template add<commands::PrefabComponentPoolCommand<TMemberHandle>>(
                    prefabComponent->entityPoolId,
                    entity.handle(),
                    prefabComponent->amount
                );

            }
        }

    };





}