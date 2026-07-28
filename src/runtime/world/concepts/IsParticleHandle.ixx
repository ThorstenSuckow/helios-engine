/**
 * @file IsParticleHandle.ixx
 * @brief Concept that matches the runtime particle handle type.
 */
module;

#include <concepts>

export module helios.engine.runtime.world.concepts.IsParticleHandle;

import helios.engine.runtime.world.types;

using namespace helios::engine::runtime::world::types;
export namespace helios::engine::runtime::world::concepts {

    /**
     * @brief Constrains a type to `ParticleHandle`.
     *
     * @tparam THandle Candidate handle type.
     */
    template<typename THandle>
    concept IsParticleHandle =
        std::is_same_v<THandle, ParticleHandle>;


}