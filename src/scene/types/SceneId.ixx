/**
 * @file SceneId.ixx
 * @brief Strong-id type definition for scenes.
 */
module;

export module helios.scene.types.SceneId;

import helios.ecs.types.StrongId;
import helios.scene.types.SceneDomainTag;

using namespace helios::ecs::types;
export namespace helios::scene::types {

    /**
     * @brief Strong id identifying scene entities.
     */
    using SceneId = StrongId<SceneDomainTag>;

}
