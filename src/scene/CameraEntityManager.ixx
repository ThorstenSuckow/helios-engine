/**
 * @file CameraEntityManager.ixx
 * @brief Camera-domain ECS registry and manager aliases.
 */
module;

export module helios.engine.scene.CameraEntityManager;

import helios.ecs.EntityManager;

import helios.engine.scene.types;

using namespace helios::engine::scene::types;
using namespace helios::ecs;
export namespace helios::engine::scene {

    /**
     * @brief Entity manager used for scene handles and scene components.
     */
    using CameraEntityManager = EntityManager<CameraHandle>;


}