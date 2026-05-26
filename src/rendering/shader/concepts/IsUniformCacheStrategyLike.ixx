/**
 * @file IsUniformCacheStrategyLike.ixx
 * @brief Concept for strategies that cache shader uniforms for a shader handle.
 */
module;

#include <concepts>

export module helios.engine.rendering.shader.concepts.IsUniformCacheStrategyLike;

import helios.engine.rendering.shader.ShaderEntity;
import helios.engine.rendering.shader.concepts.IsShaderHandle;
import helios.engine.runtime.world.EngineWorld;
import helios.engine.runtime.world.UpdateContext;

using namespace helios::engine::runtime::world;
using namespace helios::engine::rendering::shader;
export namespace helios::engine::rendering::shader::concepts {

    /**
     * @brief Constrains a strategy to provide `cacheUniforms(...)` for shader entities.
     * @tparam T Strategy type to validate.
     * @tparam TMemberHandle Shader handle type used by the strategy.
     * @details  Requires `bool cacheUniforms(TMemberHandle, RenderResourceWorld&, UpdateContext&)`
     * and a valid shader handle type via `IsShaderHandle<TMemberHandle>`.
     */
    template<typename T, typename TMemberHandle, typename... TUniformScopes>
    concept IsUniformCacheStrategyLike =
        IsShaderHandle<TMemberHandle> && (

        requires(
        T& t, TMemberHandle entityHandle, RenderResourceWorld& renderResourceWorld, UpdateContext& updateContext
    ) {
        {t.template cacheUniforms<TUniformScopes>(entityHandle, renderResourceWorld, updateContext)} -> std::same_as<bool>;
    } &&  ...);


};