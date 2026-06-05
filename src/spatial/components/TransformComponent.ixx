/**
 * @file TransformComponent.ixx
 * @brief World-space transform matrix component alias.
 */
module;

export module helios.engine.spatial.components.TransformComponent;

import helios.engine.core.components.Mat4Component;

using namespace helios::engine::core::components;
export namespace helios::engine::spatial::components {

    /**
     * @brief Domain marker for world-space transform matrices.
     */
    struct WorldMatrixDomain{};

    /**
     * @brief 4x4 matrix component representing an entity transform in world space.
     * @tparam TOwnerHandler Owner/entity handle type.
     */
    template<typename ... Args>
    using TransformComponent = Mat4Component<WorldMatrixDomain, float, Args...>;

}