module;


export module helios.engine.runtime.particle.types:ParticleTypeDefs;

import helios.ecs.common.types;

export namespace helios::engine::runtime::particle::types {

    struct ParticleDomainTag{};


    /**
     * @brief Entity handle specialized with `ParticleDomainTag`.
     *
     * @see EntityHandle
     */
    using ParticleHandle = helios::ecs::common::types::EntityHandle<ParticleDomainTag>;

}