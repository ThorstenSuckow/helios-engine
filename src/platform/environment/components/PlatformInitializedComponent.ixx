/**
 * @file PlatformInitializedComponent.ixx
 * @brief Marker component signaling completed platform initialization.
 */
module;

#include <string>

export module helios.engine.platform.environment.components.PlatformInitializedComponent;


export namespace helios::engine::platform::environment::components {

    /**
     * @brief Marker set when platform initialization finished successfully.
     *
     * @tparam THandle Runtime platform handle type.
     */
    template<typename THandle>
    struct PlatformInitializedComponent {};

} // namespace helios::engine::platform::environment::components
