/**
 * @file InstanceRenderBatchContext.ixx
 * @brief Context payload that groups instance data for one instanced draw batch.
 */

module;

#include <vector>
#include "helios-engine-config.h"

export module helios.engine.scene.types.InstanceRenderBatchContext;

import helios.ecs.common.types;
import helios.engine.rendering.mesh.types;
import helios.engine.rendering.shader.types;
import helios.engine.rendering.common.types;
import helios.engine.rendering.common.types;
import helios.math.types;

using namespace helios::ecs::common::types;
using namespace helios::engine::rendering::common::types;
using namespace helios::engine::rendering::mesh::types;
using namespace helios::engine::rendering::shader::types;
using namespace helios::engine::rendering::common::types;

export namespace helios::engine::scene::types {

    /**
     * @brief Aggregates all state required to submit one instanced render batch.
     *
     * @tparam TMemberHandle Scene member/entity handle type.
     */
    template<typename TMemberHandle, typename TRenderHandles>
    struct InstanceRenderBatchContext {

        /**
         * @brief Target render target for rendering.
         */
        typename TRenderHandles::RenderTargetHandle renderTargetHandle;

        /**
         * @brief Target viewport for rendering.
         */
        typename TRenderHandles::ViewportHandle viewportHandle;

        /**
         * @brief Owning scene handle.
         */
        typename TRenderHandles::SceneHandle sceneHandle;

        /**
         * @brief Mesh resource handle to render.
         */
        typename TRenderHandles::MeshHandle meshHandle;

        /**
         * @brief Texture resource handle to apply.
         */
        typename TRenderHandles::TextureHandle textureHandle;

        /**
         * @brief Material resource handle to apply.
         */
        typename TRenderHandles::MaterialHandle materialHandle;

        /**
         * @brief Shader resource handle used for draw submission.
         */
        typename TRenderHandles::ShaderHandle shaderHandle;

        /**
         * @brief Scene member handles contained in this instance batch.
         */
        std::vector<TMemberHandle> memberHandles;

        /**
         * @brief Per-instance payload (for example model matrices) for draw submission.
         */
        std::vector<InstanceData> instanceData;

        /**
         * @brief Constructs an instance batch context for a fixed render state tuple.
         *
         * @param renderTargetHdl Target render target handle.
         * @param viewportHdl Target viewport handle.
         * @param sceneHdl Owning scene handle.
         * @param meshHdl Mesh handle used for the batch.
         * @param textureHdl Texture handle used for the batch.
         * @param materialHdl Material handle used for the batch.
         * @param shaderHdl Shader handle used for the batch.
         */
        InstanceRenderBatchContext(
            const typename TRenderHandles::RenderTargetHandle renderTargetHdl,
            const typename TRenderHandles::ViewportHandle viewportHdl,
            const typename TRenderHandles::SceneHandle sceneHdl,
            const typename TRenderHandles::MeshHandle meshHdl,
            const typename TRenderHandles::TextureHandle textureHdl,
            const typename TRenderHandles::MaterialHandle materialHdl,
            const typename TRenderHandles::ShaderHandle shaderHdl
        ) : renderTargetHandle(renderTargetHdl),
            viewportHandle(viewportHdl),
            sceneHandle(sceneHdl),
            meshHandle(meshHdl),
            textureHandle(textureHdl),
            materialHandle(materialHdl),
            shaderHandle(shaderHdl) {
            memberHandles.reserve(DEFAULT_INSTANCE_DATA_CAPACITY);
            instanceData.reserve(DEFAULT_INSTANCE_DATA_CAPACITY);
        }
    };

}