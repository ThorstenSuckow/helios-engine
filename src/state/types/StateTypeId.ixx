
module;

#include <functional>
#include <cstddef>

export module helios.engine.state.types.StateTypeId;

import helios.core.common;


export namespace helios::engine::state::types {
    struct helios_engine_common_tag_StateTypes{};

    using StateTypeId = helios::core::common::TypeId<helios_engine_common_tag_StateTypes>;
};