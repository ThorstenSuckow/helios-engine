/**
 * @file SceneRenderContext.ixx
 * @brief Render extraction payload for a scene.
 */
module;


export module helios.engine.scene.types.SceneRenderContext;


export namespace helios::engine::scene::types {

    /**
     * @brief Render context for a viewport, representing a scene.
     */
    template<typename TMemberHandle, typename TRenderHandles>
    struct SceneRenderContext {

        using RenderTargetHandle = typename TRenderHandles::RenderTargetHandle;
        using ViewportHandle = typename TRenderHandles::ViewportHandle;
        using SceneHandle = typename TRenderHandles::SceneHandle;

        RenderTargetHandle renderTargetHandle;

        ViewportHandle viewportHandle;

        SceneHandle sceneHandle;

        friend bool operator==(const SceneRenderContext&, const SceneRenderContext&) = default;
    };

}