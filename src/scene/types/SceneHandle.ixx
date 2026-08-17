/**
 * @file SceneHandle.ixx
 * @brief Handle alias for scene entities.
 */
module;

export module helios.engine.scene.types.SceneHandle;


import helios.ecs.common.types;

import helios.engine.scene.types.SceneDomainTag;

export namespace helios::engine::scene::types {

    /**
     * @brief ECS handle type referencing scene entities.
     */
    using SceneHandle = helios::ecs::common::types::EntityHandle<SceneDomainTag>;

}
