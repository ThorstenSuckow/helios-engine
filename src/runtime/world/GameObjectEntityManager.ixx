/**
 * @file GameObjectEntityManager.ixx
 * @brief Runtime ECS registry and manager aliases for game-object entities.
 */
module;


export module helios.engine.runtime.world.GameObjectEntityManager;

import helios.ecs;
import helios.engine.runtime.world.types;


using namespace helios::ecs;
using namespace helios::ecs::strategies;
using namespace helios::engine::runtime::world::types;
export namespace helios::engine::runtime::world {

    /**
     * @brief Entity registry configuration used for game objects.
     */
    using GameObjectEntityRegistry = EntityRegistry<GameObjectDomainTag, HashedLookupStrategy<>, true>;

    /**
     * @brief Entity manager alias for game-object handles.
     */
    using GameObjectEntityManager = EntityManager<GameObjectHandle, GameObjectEntityRegistry>;

}