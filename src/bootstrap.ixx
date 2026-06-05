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

import helios.engine.platform;

import helios.engine.scene.registry;
import helios.engine.core.registry;
import helios.engine.spatial.registry;
import helios.engine.scene.registry;

import helios.engine.rendering.registry;
import helios.engine.rendering.RenderManager;
import helios.engine.runtime.registry;

import helios.engine.runtime.lifecycle;
import helios.engine.runtime.timing;
import helios.engine.runtime.enginestate;

using namespace helios::engine::state::types;
using namespace helios::engine::runtime::enginestate::types;
using namespace helios::engine::platform::environment;
using namespace helios::engine::platform::window;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::world::types;
using namespace helios::engine::runtime::gameloop;
using namespace helios::engine::runtime::messaging::command;

export namespace helios::engine::bootstrap {

    template<typename... Tuple>
    struct ComponentRegistrar;

    template<typename... TEntityManagers>
    struct ComponentRegistrar<std::tuple<TEntityManagers...>> {
        static void registerComponents() {
            (helios::engine::rendering::registerComponents<TEntityManagers>(), ...);
            (helios::engine::runtime::registerComponents<TEntityManagers>(), ...);
            (helios::engine::scene::registerComponents<TEntityManagers>(), ...);
            (helios::engine::spatial::registerComponents<TEntityManagers>(), ...);
            (helios::engine::core::registerComponents<TEntityManagers>(), ...);
        }

    };


    /**
     * @brief Registers all component types with the ComponentReflector.
     *
     * @details This function must be called during engine initialization to
     * enable runtime reflection features such as cloning, lifecycle callbacks
     * (onAcquire, onRelease, onRemove), and enable/disable toggles.
     *
     * ## Usage
     *
     * ```cpp
     * // Call once during engine startup
     * helios::engine::bootstrap::registerAllComponents();
     * ```
     *
     * @note New component types must be added to the respective module's
     *       registry.ixx file to participate in the reflection system.
     *
     * @see ComponentReflector
     * @see ComponentOpsRegistry
     */
    inline void registerAllComponents() {

        static bool done = false;
        if (done) {
            return;
        }
        done = true;

        ComponentRegistrar<RegisteredEntityManagers>::registerComponents();

    }

    /**
     * @brief Creates a pre-configured GameWorld and GameLoop pair.
     *
     * @details The factory heap-allocates both objects and performs the
     * minimal setup required before application-specific configuration:
     *
     * - Calls `registerAllComponents()` via `ComponentRegistrar`
     * - Registers `WorldLifecycleManager`, `GameStateManager`,
     *   `MatchStateManager`, and `TimerManager`
     * - Tracks `GameState` and `MatchState` in the Session
     * - Registers `RenderCommandBuffer`, `EngineCommandBuffer`,
     *   and `StateCommandBuffer`
     * - Sets initial `GameState` to `Booting`
     *
     * The caller receives ownership via `unique_ptr` and is responsible
     * for registering Managers, configuring phases/passes, calling
     * `GameWorld::init()` and `GameLoop::init()`, and driving the
     * main loop.
     *
     * ## Usage
     *
     * ```cpp
     * auto [gameWorldPtr, gameLoopPtr] = helios::engine::bootstrap::bootstrapGameWorld();
     * auto& gameWorld = *gameWorldPtr;
     * auto& gameLoop  = *gameLoopPtr;
     *
     * // Application-specific setup
     * gameWorld.registerManager<SpawnManager<GameObjectHandle>>();
     * gameLoop.phase(PhaseType::Pre)
     *     .addPass<GameState>(GameState::Any)
     *         .addSystem<InputSystem>();
     *
     * gameWorld.init();
     * gameLoop.init(gameWorld);
     * ```
     *
     * @note `registerAllComponents()` is called automatically — no separate
     *       call is required.
     *
     * @param capacity Initial capacity for the EntityManager's SparseSets.
     *                 Must be large enough to accommodate all entities including
     *                 pooled clones. Defaults to ENTITY_MANAGER_DEFAULT_CAPACITY.
     *
     * @return A pair of (GameWorld, GameLoop) unique pointers.
     *
     * @see registerAllComponents
     * @see GameWorld
     * @see GameLoop
     * @see EngineCommandBuffer
     * @see Session::trackState
     */
    inline std::pair<std::unique_ptr<GameWorld>, std::unique_ptr<GameLoop>> bootstrapGameWorld(
        const size_t capacity = ENTITY_MANAGER_DEFAULT_CAPACITY
    ) {
        auto gameWorld = std::make_unique<helios::engine::runtime::world::GameWorld>(capacity);

        auto gameLoop = std::make_unique<helios::engine::runtime::gameloop::GameLoop>(*gameWorld);


        registerAllComponents();

        // managers
        gameWorld->registerManager<helios::engine::runtime::lifecycle::WorldLifecycleManager>();

        gameWorld->registerManager<helios::engine::runtime::enginestate::EngineStateManager>(
            helios::engine::runtime::enginestate::rules::DefaultEngineStateTransitionRules::rules());

        gameWorld->registerManager<helios::engine::runtime::timing::TimerManager>();

        gameWorld->session().trackState<helios::engine::runtime::enginestate::types::EngineState>();

        gameWorld->registerCommandBuffer<RenderCommandBuffer>();
        gameWorld->registerCommandBuffer<PlatformCommandBuffer>();
        gameWorld->registerCommandBuffer<EngineCommandBuffer>();

        gameWorld->session().setStateFrom<EngineState>(
            StateTransitionContext<EngineState>(
            EngineState::Undefined,
            EngineState::Booting,
            EngineStateTransitionId::BootRequest
        ));


        return std::make_pair(std::move(gameWorld), std::move(gameLoop));
    }

}

