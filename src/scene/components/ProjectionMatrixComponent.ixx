/**
 * @file ProjectionMatrixComponent.ixx
 * @brief Alias for a computed projection matrix component.
 */
module;

export module helios.engine.scene.components.ProjectionMatrixComponent;

import helios.engine.core.components.Mat4Component;

using namespace helios::engine::core::components;
export namespace helios::engine::scene::components {

    /** @brief Domain tag for computed projection matrix values. */
    struct ProjectionMatrixTag {};

    /**
     * @brief Stores a computed projection matrix for a scene-related entity.
     */
    using ProjectionMatrixComponent = Mat4Component<ProjectionMatrixTag, float>;


}