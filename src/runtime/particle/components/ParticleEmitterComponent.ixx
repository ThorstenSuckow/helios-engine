module;

#include <cstddef>

export module helios.engine.runtime.particle.components:ParticleEmitterComponent;

import helios.engine.runtime.pooling.types;

export namespace helios::engine::runtime::particle::components {

    /**
     * @brief Component representing a particle emitter.
     *
     * @tparam TOwnerHandle The type of the entity handle that owns this component.
     */
    template<typename TOwnerHandle>
    struct ParticleEmitterComponent {
        pooling::types::EntityPoolKey entityPoolKey;
        std::size_t amount = 0;
    };
}
