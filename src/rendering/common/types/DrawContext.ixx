/**
 * @file DrawContext.ixx
 * @brief Lightweight context object for draw target selection.
 */
module;


export module helios.engine.rendering.common.types.DrawContext;


import helios.engine.rendering.common.types.ClearFlags;

import helios.math;

import helios.engine.rendering.renderTarget.types.RenderTargetHandle;
import helios.engine.rendering.viewport.types.ViewportHandle;
import helios.engine.scene.types.SceneHandle;
import helios.engine.rendering.mesh.types.MeshHandle;
import helios.engine.rendering.material.types.MaterialHandle;
import helios.engine.rendering.shader.types.ShaderHandle;

import helios.engine.rendering.common.types.ClearFlags;


using namespace helios::engine::rendering::renderTarget::types;
using namespace helios::engine::rendering::viewport::types;
using namespace helios::engine::scene::types;
using namespace helios::engine::rendering::mesh::types;
using namespace helios::engine::rendering::material::types;
using namespace helios::engine::rendering::shader::types;
using namespace helios::engine::rendering::common::types;
export namespace helios::engine::rendering::common::types {

    /**
     * @brief Groups the target handles used for a render pass.
     */
    struct DrawContext {

        RenderTargetHandle renderTargetHandle;
        ViewportHandle viewportHandle;
        SceneHandle sceneHandle;
        MeshHandle meshHandle;
        MaterialHandle materialHandle;
        ShaderHandle shaderHandle;
        helios::math::mat4f worldMatrix;

    };


}