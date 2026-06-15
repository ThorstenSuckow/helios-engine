/**
 * @file InstanceRenderBatchContext.ixx
 * @brief Context payload that groups instance data for one instanced draw batch.
 */

module;

#include <vector>
#include "helios-engine-config.h"

export module helios.engine.scene.types.InstanceRenderBatchContext;

import helios.ecs.types.EntityHandle;
import helios.engine.scene.types.SceneHandle;
import helios.engine.rendering.mesh.types;
import helios.engine.rendering.material.types;
import helios.engine.rendering.shader.types;
import helios.engine.rendering.common.types;
import helios.engine.rendering.viewport.types;
import helios.engine.rendering.renderTarget.types;
import helios.math.types;

using namespace helios::ecs::types;
using namespace helios::engine::rendering::viewport::types;
using namespace helios::engine::rendering::renderTarget::types;
using namespace helios::engine::rendering::mesh::types;
using namespace helios::engine::rendering::material::types;
using namespace helios::engine::rendering::shader::types;
using namespace helios::engine::rendering::common::types;
using namespace helios::engine::scene;

export namespace helios::engine::scene::types {

    /**
     * @brief Aggregates all state required to submit one instanced render batch.
     *
     * @tparam TMemberHandle Scene member/entity handle type.
     */
    template<typename TMemberHandle>
    struct InstanceRenderBatchContext {

        /**
         * @brief Target render target for rendering.
         */
        RenderTargetHandle renderTargetHandle;

        /**
         * @brief Target viewport for rendering.
         */
        ViewportHandle viewportHandle;

        /**
         * @brief Owning scene handle.
         */
        SceneHandle sceneHandle;

        /**
         * @brief Mesh resource handle to render.
         */
        MeshHandle meshHandle;

        /**
         * @brief Material resource handle to apply.
         */
        MaterialHandle materialHandle;

        /**
         * @brief Shader resource handle used for draw submission.
         */
        ShaderHandle shaderHandle;

        /**
         * @brief Scene member handles contained in this instance batch.
         */
        std::vector<TMemberHandle> memberHandles;

        /**
         * @brief Per-instance payload (for example model matrices) for draw submission.
         */
        std::vector<InstanceData<TMemberHandle>> instanceData;

        /**
         * @brief Constructs an instance batch context for a fixed render state tuple.
         *
         * @param renderTargetHdl Target render target handle.
         * @param viewportHdl Target viewport handle.
         * @param sceneHdl Owning scene handle.
         * @param meshHdl Mesh handle used for the batch.
         * @param materialHdl Material handle used for the batch.
         * @param shaderHdl Shader handle used for the batch.
         */
        InstanceRenderBatchContext(
            RenderTargetHandle renderTargetHdl,
            ViewportHandle viewportHdl,
            SceneHandle sceneHdl,
            MeshHandle meshHdl,
            MaterialHandle materialHdl,
            ShaderHandle shaderHdl
        ) : renderTargetHandle(renderTargetHdl),
            viewportHandle(viewportHdl),
            sceneHandle(sceneHdl),
            meshHandle(meshHdl),
            materialHandle(materialHdl),
            shaderHandle(shaderHdl) {
            memberHandles.reserve(DEFAULT_INSTANCE_DATA_CAPACITY);
            instanceData.reserve(DEFAULT_INSTANCE_DATA_CAPACITY);
        }
    };

}