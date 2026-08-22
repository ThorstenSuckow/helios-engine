/**
 * @file SwapBuffersSystem.ixx
 * @brief System that queues swap-buffer commands for shown and active windows.
 */
module;

#include <GLFW/glfw3.h>
#include <cassert>

export module helios.engine.platform.window.systems.SwapBuffersSystem;



import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.types;
import helios.engine.runtime.concepts;

import helios.ecs.command.types;
import helios.ecs.system.tags;

import helios.engine.runtime.world;
import helios.ecs;

import helios.engine.platform.window.commands;
import helios.engine.platform.window.components;
import helios.engine.platform.window.concepts.IsWindowHandle;


using namespace helios::engine::runtime::world;
using namespace helios::ecs;
using namespace helios::ecs::common::concepts;
using namespace helios::engine::platform::window::components;
using namespace helios::engine::platform::window::concepts;
using namespace helios::engine::platform::window::commands;
using namespace helios::ecs::components;
export namespace helios::engine::platform::window::systems {

    /**
     * @brief Emits `SwapBuffersCommand` for each active and shown window.
     *
     * @tparam THandle Window handle type.
     */
    template<typename THandle>
    requires IsWindowHandle<THandle>
    class SwapBuffersSystem {

    public:

        /**
         * @brief Engine role marker used by runtime registries.
         */
        using EcsRoleTag = ecs::system::tags::TypedSystemRole;
        using CommandBuffer = ecs::command::TypedCommandBuffer<SwapBuffersCommand<THandle>>;

        /**
         * @brief Enqueues swap-buffer commands for the current frame.
         *
         * @param updateContext Frame-local update context.
         */
        void update(UpdateContext& updateContext, CommandBuffer& cmdBuffer) noexcept {

            for (auto [entity, wc, wsc]: updateContext.template view<
                THandle,
                WindowComponent<THandle>,
                WindowShownComponent<THandle>
                >().withActive()) {

                cmdBuffer.template add<SwapBuffersCommand<THandle>>(entity.handle());
            }

        }

    };



}