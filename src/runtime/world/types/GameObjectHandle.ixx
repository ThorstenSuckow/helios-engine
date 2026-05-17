/**
 * @file GameObjectHandle.ixx
 * @brief Typed entity handle for runtime game objects.
 */
module;

export module helios.engine.runtime.world.types.GameObjectHandle;

import helios.ecs.types.EntityHandle;
import helios.engine.runtime.world.types.GameObjectDomainTag;

export namespace helios::engine::runtime::world::types {

    /**
     * @brief Entity handle specialized with `GameObjectDomainTag`.
     *
     * @see EntityHandle
     */
    using GameObjectHandle = helios::ecs::types::EntityHandle<GameObjectDomainTag>;

}