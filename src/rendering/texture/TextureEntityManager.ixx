/**
 * @file TextureEntityManager.ixx
 * @brief Texture-domain ECS registry and manager aliases.
 */
module;

#include "helios-engine-config.h"

export module helios.engine.rendering.texture.TextureEntityManager;

import helios.ecs.EntityManager;
import helios.ecs.EntityRegistry;
import helios.ecs.strategies;

import helios.engine.rendering.texture.types;

constexpr auto TEXTURE_SIZE = DEFAULT_TEXTURE_POOL_CAPACITY;

using namespace helios::ecs;
using namespace helios::ecs::strategies;
export namespace helios::engine::rendering::texture {

    /**
     * @brief Entity registry used for texture resources.
     */
    using TextureEntityRegistry = EntityRegistry<texture::types::TextureDomainTag, LinearLookupStrategy<TEXTURE_SIZE>, true, TEXTURE_SIZE>;

    /**
     * @brief Entity manager used for texture handles and texture components.
     */
    using TextureEntityManager = EntityManager<texture::types::TextureHandle, TextureEntityRegistry, TEXTURE_SIZE>;


}