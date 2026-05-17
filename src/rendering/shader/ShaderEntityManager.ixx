/**
 * @file ShaderEntityManager.ixx
 * @brief Shader-domain ECS registry and manager aliases.
 */
module;

#include "helios-engine-config.h"

export module helios.engine.rendering.shader.ShaderEntityManager;

import helios.ecs.EntityManager;
import helios.ecs.EntityRegistry;
import helios.ecs.strategies;

import helios.engine.rendering.shader.types;

constexpr auto SHADER_SIZE = DEFAULT_SHADER_POOL_CAPACITY;

using namespace helios::engine::rendering::shader::types;
using namespace helios::ecs;
using namespace helios::ecs::strategies;
export namespace helios::engine::rendering::shader {

    /**
     * @brief Entity registry used for shader resources.
     */
    using ShaderEntityRegistry = EntityRegistry<ShaderDomainTag, HashedLookupStrategy, true, SHADER_SIZE>;

    /**
     * @brief Entity manager used for shader handles and shader components.
     */
    using ShaderEntityManager = EntityManager<ShaderHandle, ShaderEntityRegistry, SHADER_SIZE>;


}