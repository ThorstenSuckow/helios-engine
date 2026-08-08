/**
 * @file ViewportEntityManager.ixx
 * @brief Viewport-domain ECS registry and manager aliases.
 */
module;


export module helios.engine.rendering.viewport.ViewportEntityManager;

import helios.ecs.EntityManager;
import helios.ecs.EntityRegistry;
import helios.ecs.strategies;

import helios.engine.rendering.viewport.types;

using namespace helios::engine::rendering::viewport::types;
using namespace helios::ecs;
using namespace helios::ecs::strategies;
export namespace helios::engine::rendering::viewport {

    /**
     * @brief Entity registry used for viewport resources.
     */
    using ViewportEntityRegistry = EntityRegistry<ViewportDomainTag, LinearLookupStrategy<>, true>;

    /**
     * @brief Entity manager used for viewport handles and viewport components.
     */
    using ViewportEntityManager = EntityManager<ViewportHandle, ViewportEntityRegistry>;


}