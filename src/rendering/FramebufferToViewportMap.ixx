/**
 * @file FramebufferToViewportMap.ixx
 * @brief Mapping between framebuffer handles and associated viewport handles.
 */
module;

export module helios.engine.rendering.FramebufferToViewportMap;

import helios.engine.core.container.HandleMultiMap;

import helios.engine.rendering.viewport.types.ViewportHandle;
import helios.engine.rendering.framebuffer.types.FramebufferHandle;



using namespace helios::engine::rendering::framebuffer::types;
using namespace helios::engine::rendering::viewport::types;
using namespace helios::engine::core::container;
export namespace helios::engine::rendering {

    /**
     * @brief One-to-many mapping from framebuffer handle to viewport handles.
     */
    using FramebufferToViewportMap = HandleMultiMap<FramebufferHandle, ViewportHandle>;

}