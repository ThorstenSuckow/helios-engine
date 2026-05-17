/**
 * @file WindowCloseCommand.ixx
 * @brief Command requesting closure of a specific window entity.
 */
module;

export module helios.engine.platform.window.commands.WindowCloseCommand;

import helios.engine.platform.window.types.WindowHandle;
import helios.engine.platform.window.concepts.IsWindowHandle;

using namespace helios::engine::platform::window::types;
using namespace helios::engine::platform::window::concepts;
export namespace helios::engine::platform::window::commands {

    /**
     * @brief Requests closing a single window.
     *
     * @tparam THandle Window handle type.
     */
    template<typename THandle>
    requires IsWindowHandle<THandle>
    struct WindowCloseCommand {

        /** @brief Target window entity handle. */
        THandle windowHandle;

    };


}