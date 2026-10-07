/**
 * @file Scale3DComponent.ixx
 * @brief Alias for 3D scale values.
 */
module;


export module helios.engine.spatial.components.Scale3DComponent;

import helios.engine.core.components.Vec3Component;

export namespace helios::engine::spatial::components {

    /** @brief Domain tag for 3D scale values. */
    struct Scale3DComponentDomain {};

    /**
     * @brief Stores 3D scale data in a `vec3<float>` component.
     *
     * @tparam Args Additional template arguments forwarded to the underlying component.
     */
    template<typename ...Args>
    using Scale3DComponent = helios::engine::core::components::Vec3Component<Scale3DComponentDomain, float, Args...>;

}