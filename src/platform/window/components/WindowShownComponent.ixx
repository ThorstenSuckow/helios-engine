/**
 * @file WindowShownComponent.ixx
 * @brief Marker component indicating that a window has been shown/activated.
 */
module;

export module helios.engine.platform.window.components.WindowShownComponent;

export namespace helios::engine::platform::window::components {

    /**
     * @brief Marker set once a window is visible and participates in render/present flow.
     *
     * @tparam THandle Window handle type.
     */
    template<typename THandle>
    struct WindowShownComponent {};

} // namespace helios::engine::platform::window::components
