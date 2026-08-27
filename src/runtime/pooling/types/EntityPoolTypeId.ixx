/** @file EntityPoolTypeId.ixx
 * @brief Strongly-typed identifier for EntityPool instances.
 */
module;

#include <string_view>
#include <cstddef>

export module helios.engine.runtime.pooling.types:EntityPoolTypeId;

import helios.core.common.types;

export namespace helios::engine::runtime::pooling::types {

    struct EntityPoolIdDomain{};

    using EntityPoolTypeId = core::common::types::TypeId<EntityPoolIdDomain>;

}

