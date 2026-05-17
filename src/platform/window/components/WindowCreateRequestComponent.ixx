/**
 * @file WindowCreateRequestComponent.ixx
 * @brief Component that stores deferred window creation parameters.
 */
module;

export module helios.engine.platform.window.components.WindowCreateRequestComponent;

import helios.engine.platform.window.types.WindowConfig;

using namespace helios::engine::platform::window::types;
export namespace helios::engine::platform::window::components {

    /**
     * @brief Pending request component consumed by `WindowCreateSystem`.
     *
     * @tparam THandle Window handle type.
     */
    template<typename  THandle>
    struct WindowCreateRequestComponent {

        /** @brief Window configuration used for the creation command. */
        WindowConfig windowConfig;

    };

} // namespace helios::engine::platform::window::components
