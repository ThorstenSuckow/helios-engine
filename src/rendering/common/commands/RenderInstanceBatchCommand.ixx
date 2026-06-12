/**
 * @file RenderInstanceBatchCommand.ixx
 * @brief Command payload for submitting one instanced render batch.
 */

module;



export module helios.engine.rendering.common.commands.RenderInstanceBatchCommand;

import helios.engine.scene.types.InstanceRenderBatchContext;

using namespace helios::engine::scene::types;
export namespace helios::engine::rendering::common::commands {


    /**
     * @brief Command that transports one instanced render batch to the render pipeline.
     *
     * @tparam THandle Scene member/entity handle type.
     */
    template<typename THandle>
    struct RenderInstanceBatchCommand {


        /**
         * @brief Instance batch context consumed by `RenderManager`.
         */
        InstanceRenderBatchContext<THandle> instanceRenderBatchContext;

    };


}

