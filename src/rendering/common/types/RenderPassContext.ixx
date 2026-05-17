/**
 * @file RenderPassContext.ixx
 * @brief Lightweight context object for render pass target selection.
 */
module;


export module helios.engine.rendering.common.types.RenderPassContext;


import helios.engine.rendering.common.types.ClearFlags;

import helios.math;

import helios.engine.rendering.framebuffer.types.FramebufferHandle;
import helios.engine.rendering.viewport.types.ViewportHandle;

import helios.engine.rendering.common.types.ClearFlags;

using namespace helios::engine::rendering::framebuffer::types;
using namespace helios::engine::rendering::viewport::types;
using namespace helios::engine::rendering::common::types;
export namespace helios::engine::rendering::common::types {

    /**
     * @brief Groups the target handles used for a render pass.
     */
    struct RenderPassContext {

        /** @brief Target framebuffer handle. */
        FramebufferHandle framebufferHandle{};

        /** @brief Target viewport handle. */
        ViewportHandle viewportHandle{};

    };


}