module;


export module helios.engine.runtime.particle.types:ParticleTypeDefs;

import helios.ecs.types;

using namespace helios::ecs::types;
export namespace helios::engine::runtime::particle::types {

    struct ParticleDomainTag{};

    /**
     * @brief Strong id used for game-object handle identity.
     */
    using ParticleId = StrongId<ParticleDomainTag>;

    /**
     * @brief Entity handle specialized with `ParticleDomainTag`.
     *
     * @see EntityHandle
     */
    using ParticleHandle = helios::ecs::types::EntityHandle<ParticleDomainTag>;


    /**
     * @brief Entity handle specialized with `ParticleDomainTag`.
     *
     * @see EntityHandle
     */
    using ParticleHandle = helios::ecs::types::EntityHandle<ParticleDomainTag>;
}