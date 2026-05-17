/**
 * @file GameObjectId.ixx
 * @brief Strong id type for runtime game objects.
 */
module;

export module helios.engine.runtime.world.types.GameObjectId;

import helios.ecs.types.StrongId;
import helios.engine.runtime.world.types.GameObjectDomainTag;

using namespace helios::ecs::types;
export namespace helios::engine::runtime::world::types {

    /**
     * @brief Strong id used for game-object handle identity.
     */
    using GameObjectId = StrongId<GameObjectDomainTag>;

}