/**
 * @file RenderPassContext.ixx
 * @brief Lightweight context object for render pass target selection.
 */
module;


export module helios.engine.rendering.common.types.RenderPassContext;


import helios.engine.rendering.common.types.ClearFlags;

import helios.math;

import helios.engine.rendering.renderTarget.types.RenderTargetHandle;
import helios.engine.rendering.viewport.types.ViewportHandle;

import helios.engine.rendering.common.types.ClearFlags;

using namespace helios::engine::rendering::renderTarget::types;
using namespace helios::engine::rendering::viewport::types;
using namespace helios::engine::rendering::common::types;
export namespace helios::engine::rendering::common::types {

    /**
     * @brief Groups the target handles used for a render pass.
     */
    struct RenderPassContext {

        /** @brief Target renderTarget handle. */
        RenderTargetHandle renderTargetHandle{};

        /** @brief Target viewport handle. */
        ViewportHandle viewportHandle{};

        /** @brief View Matrix used for rendering the scene. */
        helios::math::mat4f viewMatrix{};

        /** @brief Projection Matrix used for rendering the scene. */
        helios::math::mat4f projectionMatrix{};

    };


}