/**
 * @file BoundsComponent.ixx
 * @brief Alias for 4D bounds data.
 */
module;


export module helios.engine.spatial.components.BoundsComponent;

import helios.engine.core.components.Vec4Component;

export namespace helios::engine::spatial::components {

    /** @brief Domain tag for bounds values. */
    struct BoundsComponentDomain {};

    /**
     * @brief Stores bounds data in a `vec4<float>` component.
     *
     * @tparam THandle Owning entity handle type.
     */
    template<typename THandle>
    using BoundsComponent = helios::engine::core::components::Vec4Component<BoundsComponentDomain, THandle, float>;
}