/**
 * @file Rotation3DComponent.ixx
 * @brief Spatial rotation component alias based on QuaternionComponent.
 */
module;

export module helios.engine.spatial.components.Rotation3DComponent;

import helios.engine.core.components.QuaternionComponent;


export namespace helios::engine::spatial::components {

    /**
     * @brief Domain tag for 3D rotation values.
     */
    struct Rotation3DComponentDomain {};

    /**
     * @brief Stores 3D local/world rotation state as `quat<float>`.
     *
     * @tparam THandle Owning entity handle type.
     * @tparam Args Additional type tags (e.g. `Local`, `World`).
     */
    template<typename THandle, typename ...Args>
    using Rotation3DComponent = helios::engine::core::components::QuaternionComponent<
            Rotation3DComponentDomain,
            float,
            THandle,
            Args...
        >;
}