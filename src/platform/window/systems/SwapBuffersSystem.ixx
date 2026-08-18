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
     * @tparam TUpdateContextType Type of the UpdateContext to use.
     */
    template<typename THandle,
             typename TCommandBuffer = ecs::command::NullCommandBuffer,
             typename TUpdateContextType = types::SystemUpdateContext>
    requires IsWindowHandle<THandle> &&
             ecs::command::concepts::IsCommandBufferLike<TCommandBuffer> &&
             runtime::concepts::ProvidesUpdateContext<TUpdateContextType, UpdateContext>
    class SwapBuffersSystem {

    public:

        using CommandBuffer_type = TCommandBuffer;
        using UpdateContextType = TUpdateContextType;

        /**
         * @brief Engine role marker used by runtime registries.
         */
        using EcsRoleTag = ecs::system::tags::TypedSystemRole;

        /**
         * @brief Enqueues swap-buffer commands for the current frame.
         *
         * @param updateCtx Frame-local update context.
         */
        bool update(TUpdateContextType& updateCtx, TCommandBuffer& cmdBuffer) noexcept {

            auto& updateContext = updateCtx.updateContext();

            for (auto [entity, wc, wsc]: updateContext.template view<
                THandle,
                WindowComponent<THandle>,
                WindowShownComponent<THandle>
                >().withActive()) {

                cmdBuffer.template add<SwapBuffersCommand<THandle>>(entity.handle());
            }

            return true;
        }

    };



}