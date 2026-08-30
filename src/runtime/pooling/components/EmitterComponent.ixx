/**
 * @file EmitterComponent.ixx
 * @brief Component for specifying numer of entities to be emitted from a pool.
 */
module;

#include <cstdint>

export module helios.engine.runtime.pooling.components:EmitterComponent;

import helios.engine.runtime.pooling.types;

export namespace helios::engine::runtime::pooling::components {

    /**
     * @brief Component representing a particle emitter.
     *
     * @tparam TOwnerHandle The type of the entity handle that owns this component.
     */
    template<typename TOwnerHandle>
    struct EmitterComponent {
        pooling::types::EntityPoolKey entityPoolKey;
        std::uint32_t amount = 0;
    };
}
