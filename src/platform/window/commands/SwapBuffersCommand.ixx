/**
 * @file SwapBuffersCommand.ixx
 * @brief Command requesting a back/front buffer swap for a window.
 */
module;


export module helios.engine.platform.window.commands.SwapBuffersCommand;

export namespace helios::engine::platform::window::commands {

    /**
     * @brief Requests buffer swap for the given window handle.
     *
     * @tparam THandle Window handle type.
     */
    template<typename THandle>
    struct SwapBuffersCommand {

        /** @brief Target window entity handle. */
        THandle windowHandle;

    };


}