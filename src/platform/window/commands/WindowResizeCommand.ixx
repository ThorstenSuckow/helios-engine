/**
 * @file WindowResizeCommand.ixx
 * @brief Command payload for window resize updates.
 */
module;

export module helios.engine.platform.window.commands.WindowResizeCommand;


import helios.engine.rendering.renderTarget.types.RenderTargetSize;

import helios.engine.platform.window.types.WindowHandle;
import helios.engine.platform.window.types.WindowSize;
import helios.engine.platform.window.concepts.IsWindowHandle;

using namespace helios::engine::platform::window::types;
using namespace helios::engine::rendering::renderTarget::types;
using namespace helios::engine::platform::window::concepts;
export namespace helios::engine::platform::window::commands {

    /**
     * @brief Carries a resize update for a target window entity.
     *
     * @tparam THandle Window handle type.
     */
    template<typename THandle>
    requires IsWindowHandle<THandle>
    struct WindowResizeCommand {

        /** @brief Target window entity handle. */
        THandle windowHandle;

        /** @brief New window size in pixels. */
        WindowSize windowSize;

        /** @brief the underlying renderTarget size */
        RenderTargetSize renderTargetSize;

    };



}