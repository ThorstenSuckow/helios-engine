/**
 * @file DrawData.ixx
 * @brief Per-instance payload used by rendering paths.
 */

module;

export module helios.engine.rendering.common.types.DrawData;

import helios.math;

export namespace helios::engine::rendering::common::types {
    
    struct DrawData {
        helios::math::mat4f worldMatrix;
    };
}

