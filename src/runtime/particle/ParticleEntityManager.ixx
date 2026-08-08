/**
 * @file ParticleEntityManager.ixx
 * @brief Runtime ECS registry and manager aliases for particle entities.
 */
module;



export module helios.engine.runtime.particle.ParticleEntityManager;

import helios.ecs;
import helios.engine.runtime.particle.types;

using namespace helios::ecs;
using namespace helios::ecs::strategies;
using namespace helios::engine::runtime::particle::types;
export namespace helios::engine::runtime::particle {

    /**
     * @brief Entity registry configuration used for particles.
     */
    using ParticleEntityRegistry = EntityRegistry<ParticleDomainTag, HashedLookupStrategy<>, true>;

    /**
     * @brief Entity manager alias for particle handles.
     */
    using ParticleEntityManager = EntityManager<ParticleHandle, ParticleEntityRegistry>;

}