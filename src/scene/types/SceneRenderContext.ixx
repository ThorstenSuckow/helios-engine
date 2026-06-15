/**
 * @file SceneRenderContext.ixx
 * @brief Render extraction payload for a scene.
 */
module;


export module helios.engine.scene.types.SceneRenderContext;

import helios.ecs.types.EntityHandle;
import helios.engine.scene.types.SceneHandle;
import helios.engine.rendering.viewport.types;
import helios.engine.rendering.renderTarget.types;
import helios.math.types;

using namespace helios::ecs::types;
using namespace helios::engine::rendering::viewport::types;
using namespace helios::engine::rendering::renderTarget::types;
using namespace helios::engine::scene;

export namespace helios::engine::scene::types {

    /**
     * @brief Immutable render context for a viewport, representing a scene.
     *
     * @tparam THandle Scene handle type (for example game-object handle).
     */
    template<typename TMemberHandle>
    struct SceneRenderContext {

        /** @brief Render target for rendering. */
        RenderTargetHandle renderTargetHandle;

        /** @brief Target viewport for rendering. */
        ViewportHandle viewportHandle;

        /** @brief Owning scene handle. */
        SceneHandle sceneHandle;

        friend bool operator==(const SceneRenderContext<TMemberHandle>&, const SceneRenderContext<TMemberHandle>&) = default;
    };

}