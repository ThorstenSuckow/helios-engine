/**
 * @file WindowEntityManager.ixx
 * @brief ECS registry and manager aliases for the window domain.
 */
module;


#include <cstddef>
#include "helios-engine-config.h"


export module helios.engine.platform.window.WindowEntityManager;

import helios.ecs;

import helios.engine.platform.window.types;

constexpr size_t WINDOW_SIZE = DEFAULT_WINDOW_POOL_CAPACITY;

using namespace helios::ecs;
using namespace helios::ecs::strategies;
using namespace helios::engine::platform::window::types;
export namespace helios::engine::platform::window {

    /**
     * @brief Entity registry type used to allocate and validate window entities.
     */
    using WindowEntityRegistry = EntityRegistry<WindowDomainTag, LinearLookupStrategy<WINDOW_SIZE>>;

    /**
     * @brief Entity manager type used to store window-domain components.
     */
    using WindowEntityManager = EntityManager<WindowHandle, WindowEntityRegistry, WINDOW_SIZE>;

}