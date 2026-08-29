/**
 * @file RenderHandles.ixx
 * @brief List of RenderHandles used throughout the rendering system for domain partitioning,
 */
module;


export module helios.engine.rendering.RenderHandles;

export namespace helios::engine::rendering {

    template<
        typename TRenderTargetHandle,
        typename TViewportHandle,
        typename TSceneHandle,
        typename TCameraHandle,
        typename TShaderHandle,
        typename TMaterialHandle,
        typename TTextureHandle,
        typename TMeshHandle
    >
    struct RenderHandles {
        using RenderTargetHandle = TRenderTargetHandle;
        using ViewportHandle = TViewportHandle;
        using TextureHandle = TTextureHandle;
        using ShaderHandle = TShaderHandle;
        using MaterialHandle = TMaterialHandle;
        using MeshHandle = TMeshHandle;
        using SceneHandle = TSceneHandle;
        using CameraHandle = TCameraHandle;

    };


}