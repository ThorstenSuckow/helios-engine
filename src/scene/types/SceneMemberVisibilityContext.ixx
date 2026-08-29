/**
 * @file SceneMemberRenderContext.ixx
 * @brief Render extraction payload for a scene member.
 */
module;


export module helios.engine.scene.types.SceneMemberVisibilityContext;

import helios.math.types;

export namespace helios::engine::scene::types {

    /**
     * @brief Immutable render context for one scene member in one viewport.
     */
    template<typename TMemberHandle, typename TSubmissionMode, typename TRenderHandles>
    struct SceneMemberVisibilityContext {

        TMemberHandle memberHandle;

        typename TRenderHandles::RenderTargetHandle renderTargetHandle;

        typename TRenderHandles::ViewportHandle viewportHandle;

        typename TRenderHandles::SceneHandle sceneHandle;

        helios::math::mat4f worldMatrix;

        float normalizedAge = 0.0f;

    };

}