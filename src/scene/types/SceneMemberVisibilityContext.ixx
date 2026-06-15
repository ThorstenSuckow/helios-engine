/**
 * @file SceneMemberRenderContext.ixx
 * @brief Render extraction payload for a scene member.
 */
module;


export module helios.engine.scene.types.SceneMemberVisibilityContext;

import helios.ecs.types.EntityHandle;
import helios.engine.scene.types.SceneHandle;
import helios.engine.rendering.mesh.types;
import helios.engine.rendering.material.types;
import helios.engine.rendering.common.types;
import helios.engine.rendering.shader.types;
import helios.engine.rendering.viewport.types;
import helios.engine.rendering.renderTarget.types;
import helios.math.types;

using namespace helios::ecs::types;
using namespace helios::engine::rendering::viewport::types;
using namespace helios::engine::rendering::renderTarget::types;
using namespace helios::engine::rendering::mesh::types;
using namespace helios::engine::rendering::material::types;
using namespace helios::engine::rendering::shader::types;
using namespace helios::engine::scene;
using namespace helios::engine::rendering::common::types;

export namespace helios::engine::scene::types {

    /**
     * @brief Immutable render context for one scene member in one viewport.
     *
     * @tparam THandle Scene-member handle type (for example game-object handle).
     */
    template<typename TMemberHandle, typename TSubmissionMode>
    struct SceneMemberVisibilityContext {
        /**
         * @brief Handle of the scene member origin entity.
         */
        TMemberHandle memberHandle;

        /** @brief Target viewport for rendering. */
        RenderTargetHandle renderTargetHandle;

        /** @brief Target viewport for rendering. */
        ViewportHandle viewportHandle;

        /** @brief Owning scene handle. */
        SceneHandle sceneHandle;

        /** @brief World transform used for rendering the scene member. */
        helios::math::mat4f worldMatrix;


    };

}