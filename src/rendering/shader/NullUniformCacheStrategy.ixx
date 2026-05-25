/**
 * @file NullUniformCacheStrategy.ixx
 * @brief No-op uniform cache strategy that always reports success.
 */
module;


export module helios.engine.rendering.shader.NullUniformCacheStrategy;

import helios.engine.runtime.world.EngineWorld;
import helios.engine.runtime.world.UpdateContext;

using namespace helios::engine::runtime::world;
export namespace helios::opengl {

    /**
     * @brief Null-object strategy for uniform caching.
     * @tparam TMemberHandle Shader member handle type.
     * @details
     * This strategy intentionally performs no caching work and can be used as a
     * default placeholder where a cache strategy is required by templates.
     */
    template<typename TMemberHandle>
    class NullUniformCacheStrategy {


    public:


        /**
         * @brief No-op uniform caching entry point.
         * @param memberHandle Shader member handle.
         * @param renderResourceWorld Render-resource world.
         * @param updateContext Frame-local update context.
         * @return Always returns `true`.
         */
        [[nodiscard]] bool cacheUniforms(
            TMemberHandle memberHandle,
            RenderResourceWorld& renderResourceWorld,
            UpdateContext& updateContext
        ) {
            return true;
        }

    };
}