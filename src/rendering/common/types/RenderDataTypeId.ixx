/**
 * @file RenderDataTypeId.ixx
 * @brief Compile-time type identifier for render data types.
 */
module;


export module helios.engine.rendering.common.types:RenderDataTypeId;

import helios.core.common;

export namespace helios::engine::rendering::common::types {

    struct RenderDataTypeIdDomain{};

    using RenderDataTypeId = helios::core::common::types::TypeId<RenderDataTypeIdDomain>;
};