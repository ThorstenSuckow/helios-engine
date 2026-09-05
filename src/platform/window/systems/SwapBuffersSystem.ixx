/**
 * @file SwapBuffersSystem.ixx
 * @brief System that queues swap-buffer commands for shown and active windows.
 */
module;

#include <GLFW/glfw3.h>
#include <cassert>

export module helios.engine.platform.window.systems.SwapBuffersSystem;



import helios.engine.runtime.gameloop.types;
import helios.ecs.entity.EntityWorld;
import helios.ecs.entity.EntityAccessSet;
import helios.ecs.entity.Query;

import helios.ecs.command.types;


import helios.ecs;

import helios.engine.platform.window.commands;
import helios.engine.platform.window.components;



using namespace helios::ecs;
using namespace helios::ecs::common::concepts;
using namespace helios::engine::platform::window::components;
using namespace helios::engine::platform::window::commands;
using namespace helios::ecs::components;
export namespace helios::engine::platform::window::systems {

    /**
     * @brief Emits `SwapBuffersCommand` for each active and shown window.
     *
     * @tparam THandle Window handle type.
     */
    template<typename THandle>
    class SwapBuffersSystem {

        using EntityWorld = ecs::entity::EntityWorld;

        template<typename TRead, typename TWrite, typename TFilter = ecs::entity::Filter<ecs::entity::AnyDirty<>>>
        using Query = ecs::entity::Query<TRead, TWrite, TFilter>;

        template<typename ... TReads>
        using Read = ecs::entity::ReadSet<TReads...>;

        template<typename ... TWrites>
        using Write = ecs::entity::WriteSet<TWrites...>;

    public:


        using CommandBuffer = ecs::command::TypedCommandBuffer<SwapBuffersCommand<THandle>>;

        /**
         * @brief Enqueues swap-buffer commands for the current frame.
         *
         * @param query Frame-local query over shown windows.
         */
        void update(
            Query<
                Read<WindowComponent<THandle>,
                    WindowShownComponent<THandle>
                >,
                Write<>,
                ecs::entity::Filter<ecs::entity::IsActive>
            > query,
            CommandBuffer& cmdBuffer
        ) noexcept {

            for (auto [entity, wc, wsc] : query) {

                cmdBuffer.template add<SwapBuffersCommand<THandle>>(entity.handle());
            }

        }

    };



}