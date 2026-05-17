/**
 * @file PlatformCommandBuffer.ixx
 * @brief Typed command buffer for platform/window command dispatch.
 */
module;

export module helios.engine.runtime.messaging.command.PlatformCommandBuffer;



import helios.engine.platform.environment.types;
import helios.engine.platform.environment.commands;
import helios.engine.platform.window.commands;

import helios.engine.platform.lifecycle.commands;

import helios.engine.runtime.messaging.command.TypedCommandBuffer;

import helios.engine.platform.window.types.WindowHandle;

using namespace helios::engine::platform::environment::types;
using namespace helios::engine::platform::window::types;
export namespace helios::engine::runtime::messaging::command {

    /**
     * @brief Platform-focused `TypedCommandBuffer` specialization.
     *
     * @details Bundles window lifecycle commands (create/resize/swap/close)
     * and runtime platform commands (init/poll/shutdown) into a single
     * concrete command buffer type.
     */
    using PlatformCommandBuffer = helios::engine::runtime::messaging::command::TypedCommandBuffer<

            // window
            helios::engine::platform::window::commands::WindowCreateCommand<WindowHandle>,
            helios::engine::platform::window::commands::WindowResizeCommand<WindowHandle>,
            helios::engine::platform::window::commands::SwapBuffersCommand<WindowHandle>,
            helios::engine::platform::window::commands::WindowCloseCommand<WindowHandle>,

            // runtime platform
            helios::engine::platform::lifecycle::commands::PlatformInitCommand,
            helios::engine::platform::environment::commands::PollEventsCommand,
            helios::engine::platform::lifecycle::commands::ShutdownCommand
        >;

}

