/**
 * @file DrawContext.ixx
 * @brief Lightweight context object for draw target selection.
 */
module;


export module helios.engine.rendering.common.types.DrawContext;

import helios.math;

export namespace helios::engine::rendering::common::types {

    /**
     * @brief Groups the target handles used for a render pass.
     */
    template<typename TRenderHandleList>
    struct DrawContext {

        typename TRenderHandleList::RenderTargetHandleType renderTargetHandle;
        typename TRenderHandleList::ViewportHandleType viewportHandle;
        typename TRenderHandleList::SceneHandleType sceneHandle;
        typename TRenderHandleList::MeshHandleType meshHandle;
        typename TRenderHandleList::TextureHandleType textureHandle;
        typename TRenderHandleList::MaterialHandleType materialHandle;
        typename TRenderHandleList::ShaderHandleType shaderHandle;
        helios::math::mat4f worldMatrix;

    };


}