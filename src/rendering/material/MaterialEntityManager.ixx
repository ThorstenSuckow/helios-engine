/**
 * @file MaterialEntityManager.ixx
 * @brief Material-domain ECS registry and manager aliases.
 */
module;

export module helios.engine.rendering.material.MaterialEntityManager;

import helios.ecs.EntityManager;
import helios.ecs.EntityRegistry;
import helios.ecs.strategies;

import helios.engine.rendering.material.types;

using namespace helios::engine::rendering::material::types;
using namespace helios::ecs;
using namespace helios::ecs::strategies;
export namespace helios::engine::rendering::material {

    /**
     * @brief Entity registry used for material resources.
     */
    using MaterialEntityRegistry = EntityRegistry<MaterialDomainTag, HashedLookupStrategy<>, true>;

    /**
     * @brief Entity manager used for material handles and material components.
     */
    using MaterialEntityManager = EntityManager<MaterialHandle, MaterialEntityRegistry>;


}