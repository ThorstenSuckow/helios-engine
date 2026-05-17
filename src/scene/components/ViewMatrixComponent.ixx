/**
 * @file ViewMatrixComponent.ixx
 * @brief Alias for a computed view matrix component.
 */
module;

export module helios.engine.scene.components.ViewMatrixComponent;

import helios.engine.core.components.Mat4Component;

using namespace helios::engine::core::components;
export namespace helios::engine::scene::components {

    /** @brief Domain tag for computed view matrix values. */
    struct ViewMatrixTag {};

    /**
     * @brief Stores a computed view matrix for a scene-related entity.
     *
     * @tparam TOwnerHandle Owning entity handle type.
     */
    template<typename TOwnerHandle>
    using ViewMatrixComponent = Mat4Component<ViewMatrixTag, TOwnerHandle, float>;


}