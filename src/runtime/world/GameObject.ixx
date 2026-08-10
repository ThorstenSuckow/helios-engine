/**
 * @file GameObject.ixx
 * @brief Lightweight facade alias for runtime game-object entities.
 */
module;

#include <cassert>
#include <type_traits>
#include <typeindex>


export module helios.engine.runtime.world.GameObject;

import helios.ecs;
import helios.engine.runtime.world.types;

using namespace helios::ecs;
export namespace helios::engine::runtime::world {

    using GameObject = Entity<EntityManager<types::GameObjectHandle>>;

} // namespace helios
