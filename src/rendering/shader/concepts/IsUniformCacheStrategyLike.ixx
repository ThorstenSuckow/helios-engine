/**
 * @file IsUniformCacheStrategyLike.ixx
 * @brief Concept for strategies that cache shader uniforms for a shader handle.
 */
module;

#include <concepts>

export module helios.engine.rendering.shader.concepts.IsUniformCacheStrategyLike;

import helios.engine.rendering.shader.ShaderEntity;
import helios.engine.rendering.shader.concepts.IsShaderHandle;
import helios.engine.runtime.world.UpdateContext;
import helios.ecs.EntitySpace;

using namespace helios::engine::runtime::world;
using namespace helios::engine::rendering::shader;
export namespace helios::engine::rendering::shader::concepts {

    /**
     * @brief Constrains a strategy to provide `cacheUniforms(...)` for shader entities.
     * @tparam T Strategy type to validate.
     * @tparam TMemberHandle Shader handle type used by the strategy.
     * @details  Requires `bool cacheUniforms(TMemberHandle, ecs::EntitySpace&, UpdateContext&)`
     * and a valid shader handle type via `IsShaderHandle<TMemberHandle>`.
     */
    template<typename T, typename TMemberHandle, typename... TUniformScopes>
    concept IsUniformCacheStrategyLike =
        IsShaderHandle<TMemberHandle> && (

        requires(
        T& t, TMemberHandle entityHandle, ecs::EntitySpace& entitySpace
    ) {
        {t.template cacheUniforms<TUniformScopes>(entityHandle, entitySpace)} -> std::same_as<bool>;
    } &&  ...);


};