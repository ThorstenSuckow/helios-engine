/**
 * @file SceneMemberRenderContext.ixx
 * @brief Render extraction payload for a scene member.
 */
module;


export module helios.engine.scene.types.SceneMemberRenderContext;

import helios.math.types;


export namespace helios::engine::scene::types {

    /**
     * @brief Immutable render context for one scene member in one viewport.
     *
     * @tparam THandle Scene-member handle type (for example game-object handle).
     */
    template<typename TMemberHandle, typename TRenderHandles>
    struct SceneMemberRenderContext {
        /**
         * @brief Handle of the scene member origin entity.
         */
        TMemberHandle memberHandle;

        /** @brief Target viewport for rendering. */
        typename TRenderHandles::RenderTargetHandle renderTargetHandle;

        /** @brief Target viewport for rendering. */
        typename TRenderHandles::ViewportHandle viewportHandle;

        /** @brief Owning scene handle. */
        typename TRenderHandles::SceneHandle sceneHandle;

        /** @brief Mesh resource handle to render. */
        typename TRenderHandles::MeshHandle meshHandle;

        /** @brief Texture resource handle to apply. */
        typename TRenderHandles::TextureHandle textureHandle;

        /** @brief Material resource handle to apply. */
        typename TRenderHandles::MaterialHandle materialHandle;

        /** @brief Shader resource handle used for draw submission. */
        typename TRenderHandles::ShaderHandle shaderHandle;

        /** @brief World transform used for rendering the scene member. */
        helios::math::mat4f worldMatrix;

    };

}