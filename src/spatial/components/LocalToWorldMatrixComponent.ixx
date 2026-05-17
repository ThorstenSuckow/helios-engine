/**
 * @file LocalToWorldMatrixComponent.ixx
 * @brief Alias for computed local-to-world matrices.
 */
module;

export module helios.engine.spatial.components.LocalToWorldMatrixComponent;

import helios.engine.core.components.Mat4Component;

using namespace helios::engine::core::components;
export namespace helios::engine::spatial::components {

    /**
     * @brief Domain tag for computed local-to-world matrices.
     */
    struct LocalToWorldMatrixTag {};

    /**
     * @brief Matrix component storing computed local-to-world transforms.
     *
     * @tparam THandle Owning entity handle type.
     */
    template<typename THandle>
    using LocalToWorldMatrixComponent = Mat4Component<LocalToWorldMatrixTag, THandle, float>;

}