/**
 * @file RenderTargetEntityManager.ixx
 * @brief RenderTarget-domain ECS registry and manager aliases.
 */
module;

#include "helios-engine-config.h"

export module helios.engine.rendering.renderTarget.RenderTargetEntityManager;

import helios.ecs.EntityManager;
import helios.ecs.EntityRegistry;
import helios.ecs.strategies;

import helios.engine.rendering.renderTarget.types;

constexpr auto FRAMEBUFFER_SIZE = DEFAULT_FRAMEBUFFER_POOL_CAPACITY;

using namespace helios::engine::rendering::renderTarget::types;
using namespace helios::ecs;
using namespace helios::ecs::strategies;
export namespace helios::engine::rendering::renderTarget {

    /**
     * @brief Entity registry used for renderTarget resources.
     */
    using RenderTargetEntityRegistry = EntityRegistry<RenderTargetDomainTag, LinearLookupStrategy<FRAMEBUFFER_SIZE>, true, FRAMEBUFFER_SIZE>;

    /**
     * @brief Entity manager used for renderTarget handles and renderTarget components.
     */
    using RenderTargetEntityManager = EntityManager<RenderTargetHandle, RenderTargetEntityRegistry, FRAMEBUFFER_SIZE>;


}