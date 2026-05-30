/**
 * @file Position3DComponent.ixx
 * @brief Alias for vec representing 3D position vectors.
 */
module;


export module helios.engine.spatial.components.Position3DComponent;

import helios.engine.core.components.Vec3Component;

export namespace helios::engine::spatial::components {

    /** @brief Domain tag for 3D position values. */
    struct Position3DComponentDomain {};

    /**
     * @brief Stores 3d position data in a `vec3<float>` component.
     *
     * @tparam THandle Owning entity handle type.
     */
    template<typename THandle>
    using Position3DComponent = helios::engine::core::components::Vec3Component<Position3DComponentDomain, THandle, float>;
}