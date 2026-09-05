/**
 * @file WindowBasedShutdownSystem.ixx
 * @brief System that requests runtime shutdown when no active windows remain.
 */
module;

export module helios.engine.platform.lifecycle.systems.WindowBasedShutdownSystem;



import helios.engine.runtime.gameloop.types;
import helios.ecs.entity.EntityWorld;
import helios.ecs.entity.EntityAccessSet;
import helios.ecs.entity.query.Query;


import helios.ecs.command.types;

import helios.ecs;

import helios.engine.platform.window.components;

import helios.engine.platform.lifecycle.commands;
import helios.engine.platform.environment.components;



using namespace helios::ecs;
using namespace helios::ecs::common::concepts;
using namespace helios::engine::platform::environment::components;
using namespace helios::engine::platform::window::components;
using namespace helios::engine::platform::lifecycle::commands;
using namespace helios::ecs::components;

export namespace helios::engine::platform::lifecycle::systems {

    /**
     * @brief Queues `ShutdownCommand` when no active window entities are left.
     *
     * @tparam THandle Window handle type.
     */
    template<typename THandle>
    class WindowBasedShutdownSystem {

        using EntityWorld = ecs::entity::EntityWorld;

        template<typename TRead, typename TWrite, typename TFilter = ecs::entity::query::Filter<ecs::entity::query::AnyDirty<>>>
        using Query = ecs::entity::query::Query<TRead, TWrite, TFilter>;

        template<typename ... TReads>
        using Read = ecs::entity::ReadSet<TReads...>;

        template<typename ... TWrites>
        using Write = ecs::entity::WriteSet<TWrites...>;

    public:


        using CommandBuffer = ecs::command::TypedCommandBuffer<ShutdownCommand>;

        /**
         * @brief Checks window activity and queues shutdown when the set is empty.
         *
         * @param query Frame-local query over window entities.
         * @param cmdBuffer Command buffer for submitting shutdown commands.
         */
        void update(
            Query<
                Read<WindowComponent<THandle>>,
                Write<>,
                ecs::entity::query::Filter<ecs::entity::query::IsActive>
            > query,
            CommandBuffer& cmdBuffer
        ) noexcept {

            if (query.empty()) {
               cmdBuffer.template add<ShutdownCommand>();
            }

        }

    };



}