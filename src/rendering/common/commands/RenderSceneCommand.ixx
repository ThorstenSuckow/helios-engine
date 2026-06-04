/**
 * @file RenderSceneCommand.ixx
 * @brief Command wrapper for scene render work.
 */
module;



export module helios.engine.rendering.common.commands.RenderSceneCommand;

import helios.engine.scene.types.SceneRenderContext;

using namespace helios::engine::scene::types;
export namespace helios::engine::rendering::common::commands {


    /**
     * @brief Command that carries one scene render context.
     *
     * @tparam THandle Scene member handle type.
     */
    template<typename THandle>
    struct RenderSceneCommand {

        /** @brief Render context for the submitted scene. */
        const SceneRenderContext<THandle> sceneRenderContext;

    };


}

