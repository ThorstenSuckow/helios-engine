/**
 * @file RenderPass.ixx
 * @brief Render-pass payload grouping target handles and extracted render contexts.
 */
module;

#include <vector>

export module helios.engine.rendering.RenderPass;

import helios.engine.rendering.viewport.types.ViewportHandle;
import helios.engine.rendering.framebuffer.types.FramebufferHandle;
import helios.engine.scene.types.SceneMemberRenderContext;

using helios::engine::rendering::viewport::types::ViewportHandle;
using helios::engine::rendering::framebuffer::types::FramebufferHandle;
using namespace helios::engine::scene::types;

export namespace helios::engine::rendering {

    /**
     * @brief DTO representing one render pass for a framebuffer/viewport pair.
     *
     * @tparam THandle Entity handle type used by scene member render contexts.
     */
    template<typename THandle>
    struct RenderPass {

        /**
         * @brief Target viewport for this pass.
         */
        ViewportHandle viewportHandle;

        /**
         * @brief Target framebuffer for this pass.
         */
        FramebufferHandle framebufferHandle;

        /**
         * @brief Scene members to render in this pass.
         */
        std::vector<SceneMemberRenderContext<THandle>> renderContexts;

    };


}