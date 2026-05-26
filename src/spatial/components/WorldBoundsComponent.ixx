/**
 * @file WorldBoundsComponent.ixx
 * @brief World-space axis-aligned bounds component alias.
 */
module;

export module helios.engine.spatial.components.WorldBoundsComponent;

import helios.engine.core.components.AABBComponent;

using namespace helios::engine::core::components;
export namespace helios::engine::spatial::components {

    /**
     * @brief Domain marker for world-space bounds.
     */
    struct WorldBoundsDomain{};

    /**
     * @brief AABB component describing an entity's bounds in world space.
     * @tparam TOwnerHandler Owner/entity handle type.
     */
    template<typename TOwnerHandler>
    using WorldBoundsComponent = AABBComponent<WorldBoundsDomain, TOwnerHandler, float>;

}