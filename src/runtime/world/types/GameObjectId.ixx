/**
 * @file GameObjectId.ixx
 * @brief Strong id type for runtime game objects.
 */
module;

export module helios.engine.runtime.world.types.GameObjectId;

import helios.core.common.types;
import helios.engine.runtime.world.types.GameObjectDomainTag;

using namespace helios::core::common::types;
export namespace helios::engine::runtime::world::types {

    /**
     * @brief Strong id used for game-object handle identity.
     */
    using GameObjectId = StrongId<GameObjectDomainTag>;

}