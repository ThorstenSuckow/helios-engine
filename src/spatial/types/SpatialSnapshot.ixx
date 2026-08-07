/**
 * @file SpatialSnapshot.ixx
 */
module;

export module helios.engine.spatial.types:SpatialSnapshot;

import helios.math;

export namespace helios::engine::spatial::types {

    /**
     * @brief Provides aggregated spatial information.
     */
    struct SpatialSnapshot {
        math::vec3f worldPosition;
        math::aabbf worldBounds;
    };

}