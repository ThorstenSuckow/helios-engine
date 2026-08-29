/**
 * @file RenderSceneMemberCommand.ixx
 * @brief Command wrapper for extracted scene member render work.
 */
module;



export module helios.engine.rendering.common.commands.RenderSceneMemberCommand;

import helios.engine.scene.types.SceneMemberRenderContext;

using namespace helios::engine::scene::types;
export namespace helios::engine::rendering::common::commands {


    /**
     * @brief Command that carries one scene-member render context.
     *
     * @tparam THandle Scene member handle type.
     * @tparam TRenderHandles Render handle type.
     */
    template<typename THandle, typename TRenderHandles>
    struct RenderSceneMemberCommand {

        /** @brief Render context for the submitted scene member. */
        const SceneMemberRenderContext<THandle, TRenderHandles> sceneMemberRenderContext;

    };


}

