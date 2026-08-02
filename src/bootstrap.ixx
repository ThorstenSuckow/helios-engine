/**
 * @file bootstrap.ixx
 * @brief Engine bootstrap: component registration and GameWorld/GameLoop factory.
 */
module;

#include <cassert>
#include <cstddef>
#include <helios-engine-config.h>
#include <memory>
#include <utility>

export module helios.engine.bootstrap;

import helios.engine.runtime.gameloop;
import helios.engine.runtime.world;

import helios.engine.state.types.StateTransitionContext;
import helios.engine.runtime.messaging.command;

import helios.engine.runtime.world.types.GameObjectHandle;
import helios.engine.runtime.world.EntityMutationManager;

import helios.engine.platform;
import helios.engine.core.thread;

import helios.engine.rendering.RenderManager;

import helios.engine.runtime.lifecycle;
import helios.engine.runtime.pooling;
import helios.engine.runtime.particle;
import helios.engine.runtime.timing;
import helios.engine.runtime.enginestate;

using namespace helios::engine::core::thread;
using namespace helios::engine::state::types;
using namespace helios::engine::runtime::enginestate::types;
using namespace helios::engine::platform::environment;
using namespace helios::engine::platform::window;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::world::types;
using namespace helios::engine::runtime::particle;
using namespace helios::engine::runtime::particle::types;
using namespace helios::engine::runtime::gameloop;
using namespace helios::engine::runtime::messaging::command;

export namespace helios::engine::bootstrap {


    /**
     * @brief Creates a pre-configured GameWorld and GameLoop pair.
     *
     * @param jobSystem Reference to the JobSystem used for parallel system execution.
     * @param capacity Initial capacity for the EntityManager's SparseSets.
     *                 Must be large enough to accommodate all entities including
     *                 pooled clones. Defaults to ENTITY_MANAGER_DEFAULT_CAPACITY.
     *
     * @return A pair of (GameWorld, GameLoop) unique pointers.
     *
     * @see GameWorld
     * @see GameLoop
     * @see EngineCommandBuffer
     * @see Session::trackState
     */
    inline std::pair<std::unique_ptr<GameWorld>, std::unique_ptr<GameLoop>> bootstrapGameWorld(
        JobSystem& jobSystem,
        const size_t capacity = ENTITY_MANAGER_DEFAULT_CAPACITY
    ) {
        auto gameWorld = std::make_unique<helios::engine::runtime::world::GameWorld>(jobSystem, capacity);

        auto gameLoop = std::make_unique<helios::engine::runtime::gameloop::GameLoop>(*gameWorld);


        // managers
        gameWorld->registerManager<helios::engine::runtime::lifecycle::WorldLifecycleManager>();

        gameWorld->registerManager<helios::engine::runtime::enginestate::EngineStateManager>(
            helios::engine::runtime::enginestate::rules::DefaultEngineStateTransitionRules::rules());

        gameWorld->registerManager<helios::engine::runtime::timing::TimerManager>();

        // mutation manager
        gameWorld->registerManager<helios::engine::runtime::world::EntityMutationManager<GameObjectEntityManager>>(
            gameWorld->entityManager<GameObjectHandle>(),
            jobSystem
        );
        gameWorld->registerManager<helios::engine::runtime::world::EntityMutationManager<ParticleEntityManager>>(
            gameWorld->entityManager<ParticleHandle>(),
            jobSystem
        );
        gameWorld->registerManager<helios::engine::runtime::pooling::EntityPoolManager<GameObjectHandle, ParticleHandle>>(gameWorld->engineWorld());

        gameWorld->session().trackState<helios::engine::runtime::enginestate::types::EngineState>();

        gameWorld->registerCommandBuffer<RenderCommandBuffer>();
        gameWorld->registerCommandBuffer<PlatformCommandBuffer>();
        gameWorld->registerCommandBuffer<EngineCommandBuffer>();
        gameWorld->registerCommandBuffer<EntityPoolCommandBuffer>();

        gameWorld->registerCommandBuffer<EntityMutationCommandBuffer<GameObjectEntityManager>>();
        gameWorld->registerCommandBuffer<EntityMutationCommandBuffer<ParticleEntityManager>>();

        gameWorld->session().setStateFrom<EngineState>(
            StateTransitionContext<EngineState>(
            EngineState::Undefined,
            EngineState::Booting,
            EngineStateTransitionId::BootRequest
        ));


        return std::make_pair(std::move(gameWorld), std::move(gameLoop));
    }

}

