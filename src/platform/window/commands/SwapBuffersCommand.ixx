/**
 * @file SwapBuffersCommand.ixx
 * @brief Command requesting a back/front buffer swap for a window.
 */
module;


export module helios.engine.platform.window.commands.SwapBuffersCommand;

import helios.engine.platform.window.types.WindowHandle;
import helios.engine.platform.window.concepts;

using namespace helios::engine::platform::window::types;
using namespace helios::engine::platform::window::concepts;
export namespace helios::engine::platform::window::commands {

    /**
     * @brief Requests buffer swap for the given window handle.
     *
     * @tparam THandle Window handle type.
     */
    template<typename THandle>
    requires IsWindowHandle<THandle>
    struct SwapBuffersCommand {

        /** @brief Target window entity handle. */
        THandle windowHandle;

    };


}