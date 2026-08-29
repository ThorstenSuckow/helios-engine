/**
 * @file WindowCreateCommand.ixx
 * @brief Command payload for creating a platform window.
 */
module;

#include <string>

export module helios.engine.platform.window.commands.WindowCreateCommand;

import helios.engine.platform.window.types.WindowConfig;

using namespace helios::engine::platform::window::types;
export namespace helios::engine::platform::window::commands {

    /**
     * @brief Requests creation of a window for a specific window entity.
     *
     * @tparam THandle Window handle type.
     */
    template<typename THandle>
    struct WindowCreateCommand {
        /** @brief Target window entity handle. */
        THandle windowHandle;

        /** @brief Creation configuration payload. */
        WindowConfig windowConfig;
    };


}