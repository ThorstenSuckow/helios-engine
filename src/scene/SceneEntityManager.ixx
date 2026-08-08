/**
 * @file SceneEntityManager.ixx
 * @brief Scene-domain ECS registry and manager aliases.
 */
module;

export module helios.engine.scene.SceneEntityManager;

import helios.ecs.EntityManager;
import helios.ecs.EntityRegistry;
import helios.ecs.strategies;

import helios.engine.scene.types;

using namespace helios::engine::scene::types;
using namespace helios::ecs;
using namespace helios::ecs::strategies;
export namespace helios::engine::scene {

    /**
     * @brief Entity registry used for scene resources.
     */
    using SceneEntityRegistry = EntityRegistry<SceneDomainTag, LinearLookupStrategy<>, true>;

    /**
     * @brief Entity manager used for scene handles and scene components.
     */
    using SceneEntityManager = EntityManager<SceneHandle, SceneEntityRegistry>;


}