/**
 * @file InstanceData.ixx
 * @brief Per-instance payload used by instanced rendering paths.
 */

module;

export module helios.engine.rendering.common.types.InstanceData;

import helios.math;

export namespace helios::engine::rendering::common::types {

    /**
     * @brief Instance payload transferred to the render backend.
     */
    template<typename TOwnerHandle>
    struct InstanceData {
        /**
         * @brief Model transform matrix for one rendered instance.
         */
        math::mat4f modelMatrix;
    };
}

