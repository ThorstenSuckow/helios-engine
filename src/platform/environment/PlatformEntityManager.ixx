/**
 * @file PlatformEntityManager.ixx
 * @brief ECS registry and manager aliases for runtime platform entities.
 */
module;


#include <cstddef>
#include "helios-engine-config.h"

export module helios.engine.platform.environment.PlatformEntityManager;

import helios.ecs;

import helios.engine.platform.environment.types;

constexpr size_t PLATFORM_SIZE = 1;

using namespace helios::ecs;
using namespace helios::ecs::strategies;
using namespace helios::engine::platform::environment::types;
export namespace helios::engine::platform::environment {

    /**
     * @brief Registry type for runtime platform entity lifecycle management.
     */
    using PlatformEntityRegistry = EntityRegistry<PlatformDomainTag, LinearLookupStrategy<PLATFORM_SIZE>, false, PLATFORM_SIZE>;

    /**
     * @brief Entity manager type for runtime platform components.
     */
    using PlatformEntityManager = EntityManager<PlatformHandle, PlatformEntityRegistry, PLATFORM_SIZE>;

}