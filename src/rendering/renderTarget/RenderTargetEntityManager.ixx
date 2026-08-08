/**
 * @file RenderTargetEntityManager.ixx
 * @brief RenderTarget-domain ECS registry and manager aliases.
 */
module;

export module helios.engine.rendering.renderTarget.RenderTargetEntityManager;

import helios.ecs.EntityManager;
import helios.ecs.EntityRegistry;
import helios.ecs.strategies;

import helios.engine.rendering.renderTarget.types;

using namespace helios::engine::rendering::renderTarget::types;
using namespace helios::ecs;
using namespace helios::ecs::strategies;
export namespace helios::engine::rendering::renderTarget {

    /**
     * @brief Entity registry used for renderTarget resources.
     */
    using RenderTargetEntityRegistry = EntityRegistry<RenderTargetDomainTag, LinearLookupStrategy<>, true>;

    /**
     * @brief Entity manager used for renderTarget handles and renderTarget components.
     */
    using RenderTargetEntityManager = EntityManager<RenderTargetHandle, RenderTargetEntityRegistry>;


}