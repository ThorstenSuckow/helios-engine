/**
 * @file CameraHandle.ixx
 * @brief Handle alias for camera entities.
 */
module;

export module helios.engine.scene.types.CameraHandle;


import helios.ecs.common.types;

import helios.engine.scene.types.CameraDomainTag;

export namespace helios::engine::scene::types {

    /**
     * @brief ECS handle type referencing scene entities.
     */
    using CameraHandle = helios::ecs::common::types::EntityHandle<CameraDomainTag>;

}
