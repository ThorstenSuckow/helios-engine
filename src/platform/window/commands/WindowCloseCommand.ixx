/**
 * @file WindowCloseCommand.ixx
 * @brief Command requesting closure of a specific window entity.
 */
module;

export module helios.engine.platform.window.commands.WindowCloseCommand;

export namespace helios::engine::platform::window::commands {

    /**
     * @brief Requests closing a single window.
     *
     * @tparam THandle Window handle type.
     */
    template<typename THandle>
    struct WindowCloseCommand {

        /** @brief Target window entity handle. */
        THandle windowHandle;

    };


}