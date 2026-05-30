/**
 * @file RenderTargetToViewportMap.ixx
 * @brief Mapping between renderTarget handles and associated viewport handles.
 */
module;

export module helios.engine.rendering.RenderTargetToViewportMap;

import helios.engine.core.container.HandleMultiMap;

import helios.engine.rendering.viewport.types.ViewportHandle;
import helios.engine.rendering.renderTarget.types.RenderTargetHandle;



using namespace helios::engine::rendering::renderTarget::types;
using namespace helios::engine::rendering::viewport::types;
using namespace helios::engine::core::container;
export namespace helios::engine::rendering {

    /**
     * @brief One-to-many mapping from renderTarget handle to viewport handles.
     */
    using RenderTargetToViewportMap = HandleMultiMap<RenderTargetHandle, ViewportHandle>;

}