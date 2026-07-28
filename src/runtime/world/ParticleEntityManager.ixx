/**
 * @file ParticleEntityManager.ixx
 * @brief Runtime ECS registry and manager aliases for particle entities.
 */
module;


#include <cstddef>
#include "helios-engine-config.h"


export module helios.engine.runtime.world.ParticleEntityManager;

import helios.ecs;
import helios.engine.runtime.world.types;

constexpr size_t PARTICLE_SIZE = DEFAULT_PARTICLE_CAPACITY;


using namespace helios::ecs;
using namespace helios::ecs::strategies;
using namespace helios::engine::runtime::world::types;
export namespace helios::engine::runtime::world {

    /**
     * @brief Entity registry configuration used for particles.
     */
    using ParticleEntityRegistry = EntityRegistry<ParticleDomainTag, HashedLookupStrategy, true, PARTICLE_SIZE>;

    /**
     * @brief Entity manager alias for particle handles.
     */
    using ParticleEntityManager = EntityManager<ParticleHandle, ParticleEntityRegistry, PARTICLE_SIZE>;

}