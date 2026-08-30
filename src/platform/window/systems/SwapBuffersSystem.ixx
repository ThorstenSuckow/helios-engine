/**
 * @file SwapBuffersSystem.ixx
 * @brief System that queues swap-buffer commands for shown and active windows.
 */
module;

#include <GLFW/glfw3.h>
#include <cassert>

export module helios.engine.platform.window.systems.SwapBuffersSystem;



import helios.engine.runtime.gameloop.types;
import helios.ecs.EcsWorld;

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

        using EcsWorld = ecs::EcsWorld;

    public:


        using CommandBuffer = ecs::command::TypedCommandBuffer<SwapBuffersCommand<THandle>>;

        /**
         * @brief Enqueues swap-buffer commands for the current frame.
         *
         * @param ecsWorld Frame-local ECS world.
         */
        void update(EcsWorld& ecsWorld, CommandBuffer& cmdBuffer) noexcept {

            for (auto [entity, wc, wsc]: ecsWorld.view<
                THandle,
                WindowComponent<THandle>,
                WindowShownComponent<THandle>
                >().withActive()) {

                cmdBuffer.template add<SwapBuffersCommand<THandle>>(entity.handle());
            }

        }

    };



}