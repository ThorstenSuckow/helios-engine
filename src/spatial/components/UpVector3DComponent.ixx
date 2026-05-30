/**
 * @file UpVector3DComponent.ixx
 * @brief Alias for vec representing 3D UpVector vectors.
 */
module;


export module helios.engine.spatial.components.UpVector3DComponent;

import helios.engine.core.components.Vec3Component;

export namespace helios::engine::spatial::components {

    /** @brief Domain tag for 3D UpVector values. */
    struct UpVector3DComponentDomain {};

    /**
     * @brief Stores 3d UpVector data in a `vec3<float>` component.
     *
     * @tparam THandle Owning entity handle type.
     */
    template<typename THandle>
    using UpVector3DComponent = helios::engine::core::components::Vec3Component<UpVector3DComponentDomain, THandle, float>;
}