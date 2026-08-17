/**
 * @file SystemTypeId.ixx
 * @brief Unique type identifier for system types.
 */
module;

export module helios.engine.runtime.world.types.SystemTypeId;

import helios.core.common;

export namespace helios::engine::runtime::world::types {

    struct helios_engine_common_tag_SystemTypes{};

    using SystemTypeId = core::common::TypeId<helios_engine_common_tag_SystemTypes>;

};