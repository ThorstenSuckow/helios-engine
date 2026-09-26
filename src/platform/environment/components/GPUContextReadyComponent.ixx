/**
 * @file GPUContextReadyComponent.ixx
 * @brief Marker component signaling GPU context readiness.
 */
module;

#include <string>

export module helios.engine.platform.environment.components.GPUContextReadyComponent;


export namespace helios::engine::platform::environment::components {

    /**
     * @brief Marker set once GPU/context-dependent resources can be used.
     *
     * @tparam THandle Runtime platform handle type.
     */
    struct GPUContextReadyComponent {

    };

} // namespace helios::engine::platform::environment::components
