/**
 * @file TargetPosition3DComponent.ixx
 * @brief Alias for vec representing 3D TargetPosition vectors.
 */
module;


export module helios.engine.spatial.components.TargetPosition3DComponent;

import helios.engine.core.components.Vec3Component;

export namespace helios::engine::spatial::components {

    /** @brief Domain tag for 3D TargetPosition values. */
    struct TargetPosition3DComponentDomain {};

    /**
     * @brief Stores 3d TargetPosition data in a `vec3<float>` component.
     *
     * @tparam THandle Owning entity handle type.
     */
    template<typename THandle>
    using TargetPosition3DComponent = helios::engine::core::components::Vec3Component<TargetPosition3DComponentDomain, THandle, float>;
}