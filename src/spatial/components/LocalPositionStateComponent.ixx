/**
 * @file LocalPositionStateComponent.ixx
 * @brief Alias for local-space position state.
 */
module;

export module helios.engine.spatial.components.LocalPositionStateComponent;

import helios.engine.core.components.Vec3Component;

using namespace helios::engine::core::components;
export namespace helios::engine::spatial::components {

    /**
     * @brief Domain tag for local position state.
     */
    struct LocalPositionStateTag {};

    /**
     * @brief Vector component storing local-space position state.
     *
     * @tparam THandle Owning entity handle type.
     */
    template<typename THandle>
    using LocalPositionStateComponent = Vec3Component<LocalPositionStateTag, THandle, float>; 

}