/**
 * @file Direction3DComponent.ixx
 * @brief Alias for vec representing 3D direction vectors.
 */
module;


export module helios.engine.spatial.components.Direction3DComponent;

import helios.engine.core.components.Vec3Component;

export namespace helios::engine::spatial::components {

    /** @brief Domain tag for 3D direction values. */
    struct Direction3DComponentDomain {};

    /**
     * @brief Stores 3d direction data in a `vec3<float>` component.
     *
     * @tparam THandle Owning entity handle type.
     */
    template<typename THandle>
    using Direction3DComponent = helios::engine::core::components::Vec3Component<Direction3DComponentDomain, float, THandle>;
}