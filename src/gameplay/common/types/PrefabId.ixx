/**
 * @file PrefabId.ixx
 * @brief Strongly-typed identifier for prefab templates.
 */
module;

#include <cstdint>
#include <string_view>

export module helios.gameplay.common.types.PrefabId;

import helios.core.types.FuncDefs;
import helios.core.types;
import helios.ecs.types.StrongId;

export namespace helios::gameplay::common::types {

    /**
     * @brief Tag type for PrefabId.
     */
    struct PrefabIdTag{};

    /**
     * @brief Strongly-typed identifier for prefabs.
     *
     * @details Used to uniquely identify prefabs.
     *
     * @see helios::ecs::types::StrongId
     */
    using PrefabId = helios::ecs::types::StrongId<PrefabIdTag>;

}
