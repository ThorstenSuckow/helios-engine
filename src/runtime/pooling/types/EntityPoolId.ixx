/** @file EntityPoolId.ixx
 * @brief Strongly-typed identifier for EntityPool instances.
 */
module;

#include <string_view>
#include <cstddef>

export module helios.engine.runtime.pooling.types:EntityPoolId;

import helios.core.common.types;
import helios.ecs.common.types;

export namespace helios::engine::runtime::pooling::types {

    /**
     * @brief Tag type for EntityPoolId.
     */
    template<typename THandle>
    struct EntityPoolIdTag{};

    /**
     * @brief Strongly-typed identifier for referencing a EntityPool.
     *
     * @details Provides type-safety when working with multiple object pools.
     * Uses FNV-1a hashing for compile-time string-based construction.
     *
     * @see helios::core::common::types::StrongId
     * @see SpawnProfileId
     * @see SpawnRuleId
     */
    template<typename THandle>
    using EntityPoolId = helios::core::common::types::StrongId<EntityPoolIdTag<THandle>>;

}

/**
 * @brief Hash specialization for EntityPoolId.
 */
template<typename THandle>
struct std::hash<helios::engine::runtime::pooling::types::EntityPoolId<THandle>> {
    std::size_t operator()(const helios::engine::runtime::pooling::types::EntityPoolId<THandle>& id) const noexcept {
        return id.value();
    }
};
