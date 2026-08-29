/**
 * @file RenderSceneCommand.ixx
 * @brief Command wrapper for scene render work.
 */
module;

#include <memory>

export module helios.engine.rendering.common.commands.RenderSceneCommand;

import helios.engine.scene.types.SceneRenderContext;

export namespace helios::engine::rendering::common::commands {


    /**
     * @brief Command that carries one scene render context.
     *
     * @tparam THandle Scene member handle type.
     */
    template<typename THandle, typename TRenderHandles>
    struct RenderSceneCommand {

        /** @brief Render context for the submitted scene. */
        const scene::types::SceneRenderContext<THandle, TRenderHandles> sceneRenderContext;

    };


}

