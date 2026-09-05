/**
 * @file WindowCreateSystem.ixx
 * @brief System that translates window-create request components into window-create commands.
 */
module;

export module helios.engine.platform.window.systems.WindowCreateSystem;



import helios.engine.runtime.gameloop.types;
import helios.ecs.command;

import helios.ecs.entity.EntityWorld;
import helios.ecs.entity.EntityAccessSet;
import helios.ecs.entity.Query;

import helios.engine.platform.window.components.WindowCreateRequestComponent;
import helios.engine.platform.window.commands.WindowCreateCommand;

import helios.ecs.component;


using namespace helios::ecs::command::concepts;
using namespace helios::ecs;
using namespace helios::engine::platform::window::components;
using namespace helios::engine::platform::window::commands;
using namespace helios::ecs::components;
using namespace helios::engine::platform::window::components;
export namespace helios::engine::platform::window::systems {

    /**
     * @brief Queues `WindowCreateCommand` for active entities with pending create requests.
     *
     * @tparam THandle Window-domain entity handle type.
     */
    template<typename THandle>
    class WindowCreateSystem {

        using UpdateContext = engine::runtime::gameloop::types::UpdateContext;

        template<typename TRead, typename TWrite, typename TFilter = ecs::entity::Filter<ecs::entity::AnyDirty<>>>
        using Query = ecs::entity::Query<TRead, TWrite, TFilter>;

        template<typename ... TReads>
        using Read = ecs::entity::ReadSet<TReads...>;

        template<typename ... TWrites>
        using Write = ecs::entity::WriteSet<TWrites...>;

        public:


        using CommandBuffer = ecs::command::TypedCommandBuffer<WindowCreateCommand<THandle>>;

        void update(
            Query<
                Read<WindowCreateRequestComponent<THandle>>,
                Write<>,
                ecs::entity::Filter<ecs::entity::IsActive>
            > query,
            CommandBuffer& cmdBuffer
        ) noexcept {

            for (auto [entity, win] : query) {

                cmdBuffer.template add<WindowCreateCommand<THandle>>(
                    entity.handle(),
                    win->windowConfig
                );
            }
        }

    };



}