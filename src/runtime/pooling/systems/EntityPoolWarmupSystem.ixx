/**
 * @file EntityPoolWarmupSystem.ixx
 * @brief System that translates PrefabRequestComponent into PrefabComponentPoolCommand.
 */
module;


export module helios.engine.runtime.pooling.systems:EntityPoolWarmupSystem;


import helios.ecs.common;
import helios.ecs.command;
import helios.ecs.entity.EntityWorld;
import helios.ecs.entity.EntityAccessSet;
import helios.ecs.entity.query.Query;

import helios.engine.runtime.pooling.commands;
import helios.engine.runtime.pooling.types;
import helios.engine.runtime.pooling.components;

export namespace helios::engine::runtime::pooling::systems {

    /**
     * @brief System for creating PrewarmEntityPoolCommand from PrefabEntityPoolRequestComponent.
     *
     * @tparam TMemberHandle The type of the member handle.
     */
    template<typename TMemberHandle>
    class EntityPoolWarmupSystem {

        template<typename TRead, typename TWrite, typename TFilter = ecs::entity::query::Filter<ecs::entity::query::AnyDirty<>>>
        using Query = ecs::entity::query::Query<TRead, TWrite, TFilter>;

        template<typename ... TReads>
        using Read = ecs::entity::ReadSet<TReads...>;

        template<typename ... TWrites>
        using Write = ecs::entity::WriteSet<TWrites...>;

    public:

        using CommandBuffer = ecs::command::TypedCommandBuffer<commands::PrewarmEntityPoolCommand<TMemberHandle>>;


        /**
         * @brief Picks up any PrefabEntityPoolRequestComponent and creates a PrewarmEntityPoolCommand.
         *
         * @details Subsequent managers are responsible for removing the entity to
         * prevent multiple warmup attempts.
         *
         * @param query Query over pool warmup request/key components.
         * @param cmdBuffer The command buffer receiving the emitted pool commands.
         */
        void update(
            Query<
                Read<components::PrefabEntityPoolRequestComponent<TMemberHandle>,
                    components::EntityPoolKeyComponent<TMemberHandle>
                >,
                Write<>,
                ecs::entity::query::Filter<ecs::entity::query::IsActive>
            > query,
            CommandBuffer& cmdBuffer
        ) noexcept {

            for (auto [entity, requestComponent, keyComponent] : query) {
                cmdBuffer.template add<commands::PrewarmEntityPoolCommand<TMemberHandle>>(
                    keyComponent->entityPoolKey,
                    entity.handle(),
                    requestComponent->value()
                );

            }
        }

    };





}