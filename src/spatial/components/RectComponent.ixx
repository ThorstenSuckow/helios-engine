/**
 * @file RectComponent.ixx
 * @brief Alias for 2D rectangular data.
 */
module;


export module helios.engine.spatial.components.RectComponent;

import helios.engine.core.components.Vec4Component;

export namespace helios::engine::spatial::components {

    /** @brief Domain tag for rectangular values. */
    struct RectComponentDomain {};

    /**
     * @brief Stores 2D rect data (x1, y1, x2, y2) in a `vec4<float>` component.
     *
     * @tparam THandle Owning entity handle type.
     */
    template<typename ... Args>
    using RectComponent = helios::engine::core::components::Vec4Component<RectComponentDomain, float, Args...>;
}