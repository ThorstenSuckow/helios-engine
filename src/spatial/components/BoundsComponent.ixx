/**
 * @file BoundsComponent.ixx
 * @brief space axis-aligned bounds component alias.
 */
module;

export module helios.engine.spatial.components.BoundsComponent;

import helios.engine.core.components.AABBComponent;

using namespace helios::engine::core::components;
export namespace helios::engine::spatial::components {

    /**
     * @brief Domain marker for -space bounds.
     */
    struct BoundsDomain{};

    /**
     * @brief AABB component describing an entity's bounds in  space.
     * @tparam TOwnerHandler Owner/entity handle type.
     */
    template<typename ... Args>
    using BoundsComponent = AABBComponent<BoundsDomain, float, Args...>;

}