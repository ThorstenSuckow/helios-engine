/**
 * @file SwapBuffersSystem.ixx
 * @brief System that queues swap-buffer commands for shown and active windows.
 */
module;

#include <GLFW/glfw3.h>
#include <cassert>

export module helios.engine.platform.window.systems.SwapBuffersSystem;



import helios.engine.runtime.world.UpdateContext;

import helios.engine.runtime.world.tags.SystemRole;

import helios.engine.runtime.world;
import helios.ecs;

import helios.engine.platform.window.commands;
import helios.engine.platform.window.components;
import helios.engine.platform.window.concepts.IsWindowHandle;

using namespace helios::engine::runtime::world::tags;
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
    template<typename THandle, typename TCommandBuffer = ecs::command::NullCommandBuffer>
    requires IsWindowHandle<THandle> && ecs::command::concepts::IsCommandBufferLike<TCommandBuffer>
    class SwapBuffersSystem {

    public:

        using CommandBuffer_type = TCommandBuffer;

        /**
         * @brief Engine role marker used by runtime registries.
         */
        using EcsRoleTag = TypedSystemRole;

        /**
         * @brief Enqueues swap-buffer commands for the current frame.
         *
         * @param updateContext Frame-local update context.
         */
        void update(UpdateContext& updateContext, TCommandBuffer& cmdBuffer) noexcept {

            for (auto [entity, wc, wsc]: updateContext.view<
                THandle,
                WindowComponent<THandle>,
                WindowShownComponent<THandle>
                >().withActive()) {

                cmdBuffer.template add<SwapBuffersCommand<THandle>>(entity.handle());
            }

        }

    };



}