/**
 * @file CurrentContextComponent.ixx
 * @brief Marker component identifying the currently active platform context.
 */
module;

#include <string>

export module helios.engine.platform.environment.components.CurrentContextComponent;


export namespace helios::engine::platform::environment::components {

    /**
     * @brief Marker assigned to the entity owning the current platform context.
     *
     * @tparam THandle Runtime platform handle type.
     */
    struct CurrentContextComponent {

    };

} // namespace helios::engine::platform::environment::components
