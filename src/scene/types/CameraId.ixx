/**
 * @file CameraId.ixx
 * @brief Strong-id type definition for cameras.
 */
module;

export module helios.engine.scene.types.CameraId;

import helios.ecs.types.StrongId;
import helios.engine.scene.types.CameraDomainTag;

using namespace helios::ecs::types;
export namespace helios::engine::scene::types {

    /**
     * @brief Strong id identifying camera entities.
     */
    using CameraId = StrongId<CameraDomainTag>;

}
