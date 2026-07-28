/**
 * @file CameraEntityManager.ixx
 * @brief Camera-domain ECS registry and manager aliases.
 */
module;

#include "helios-engine-config.h"

export module helios.engine.scene.CameraEntityManager;

import helios.ecs.EntityManager;
import helios.ecs.EntityRegistry;
import helios.ecs.strategies;

import helios.engine.scene.types;

constexpr auto CAMERA_SIZE = DEFAULT_CAMERA_POOL_CAPACITY;

using namespace helios::engine::scene::types;
using namespace helios::ecs;
using namespace helios::ecs::strategies;
export namespace helios::engine::scene {

    /**
     * @brief Entity registry used for camera resources.
     */
    using CameraEntityRegistry = EntityRegistry<CameraDomainTag, LinearLookupStrategy<CAMERA_SIZE>, true, CAMERA_SIZE>;

    /**
     * @brief Entity manager used for scene handles and scene components.
     */
    using CameraEntityManager = EntityManager<CameraHandle, CameraEntityRegistry, CAMERA_SIZE>;


}