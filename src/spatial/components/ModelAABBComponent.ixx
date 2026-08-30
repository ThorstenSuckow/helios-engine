/**
 * @file ModelAabbComponent.ixx
 * @brief Component for storing the canonical AABB of a model.
 */
module;

export module helios.engine.spatial.components.ModelAABBComponent;

import helios.math.types;

import helios.engine.core.components;

export namespace helios::engine::spatial::components {

    struct ModelAABBComponent {};

    template<typename THandle, typename ... T>
    using ModelAabbComponent = helios::engine::core::components::AABBComponent<
        ModelAABBComponent, float, THandle, T...>;

}