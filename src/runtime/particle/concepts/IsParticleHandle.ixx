/**
 * @file IsParticleHandle.ixx
 * @brief Concept that matches the runtime particle handle type.
 */
module;

#include <concepts>

export module helios.engine.runtime.particle.concepts:IsParticleHandle;

import helios.engine.runtime.particle.types;

using namespace helios::engine::runtime::particle::types;
export namespace helios::engine::runtime::particle::concepts {

    /**
     * @brief Constrains a type to `ParticleHandle`.
     *
     * @tparam THandle Candidate handle type.
     */
    template<typename THandle>
    concept IsParticleHandle =
        std::is_same_v<THandle, ParticleHandle>;


}