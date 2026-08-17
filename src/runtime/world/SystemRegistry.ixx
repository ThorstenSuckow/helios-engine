/**
 * @file SystemRegistry.ixx
 * @brief Type-indexed registry for managing System instances within a game loop pass.
 */
module;

#include <cassert>
#include <memory>
#include <span>
#include <vector>


export module helios.engine.runtime.world.SystemRegistry;

import helios.core.container;
import helios.engine.runtime.world.System;
import helios.engine.runtime.world.types.SystemTypeId;


using namespace helios::core::container;
using namespace helios::engine::runtime::world::types;

export namespace helios::engine::runtime::world {

    /**
     * @brief Type alias for a ConceptModelRegistry specialized for Systems.
     *
     * @see ConceptModelRegistry
     * @see System
     */
    using SystemRegistry = ConceptModelRegistry<System, SystemTypeId>;

}