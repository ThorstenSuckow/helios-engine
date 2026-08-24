/**
 * @file GameLoop.ixx
 * @brief Central orchestrator for the game update cycle.
 */
module;

#include <cassert>
#include <memory>
#include <vector>
#include <span>

export module helios.engine.runtime.gameloop:GameLoop;

import helios.engine.runtime.world.GameWorld;

import helios.engine.runtime.world.UpdateContext;
import helios.core.log.Logger;
import helios.core.log.LogManager;

import helios.ecs;

import helios.engine.runtime.enginestate.types;

import :Pass;
import :Phase;

import helios.engine.input.InputSnapshot;

import helios.engine.runtime.world.GameWorld;

using namespace helios::engine::runtime::world;

#define HELIOS_LOG_SCOPE "GameLoop"
export namespace helios::engine::runtime::gameloop {

    /**
     * @brief Central orchestrator for the game update cycle.
     */
    class GameLoop {


        /**
         * @brief Flag indicating whether init() has been called.
         *
         * Used to assert that init() is called exactly once before the first update()
         * and to prevent multiple initializations.
         */
        bool initialized_ = false;

        inline static const helios::core::log::Logger& logger_ = helios::core::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);

        GameWorld& gameWorld_;



        ecs::common::container::EcsDataContainer ecsDataContainer_{true};

        Phase prePhase_;
        Phase mainPhase_;
        Phase postPhase_;

        float totalTime_ = 0.0f;

        std::size_t frameCount_ = 0;


    public:

        /**
         * @brief Constructs a GameLoop bound to a GameWorld.
         *
         * @param gameWorld The GameWorld associated with this GameLoop.
         *
         */
        GameLoop(GameWorld& gameWorld) :
            gameWorld_(gameWorld),
            prePhase_(gameWorld_),
            mainPhase_(gameWorld_),
            postPhase_(gameWorld_) {};


        /**
         * @brief Returns a reference to the specified phase.
         *
         * @param phaseType The type of phase to retrieve (Pre, Main, or Post).
         *
         * @return Reference to the requested Phase.
         */
        [[nodiscard]] Phase& phase(const PhaseType phaseType) noexcept {

            switch (phaseType) {
                case PhaseType::Pre:
                    return prePhase_;
                    break;
                case PhaseType::Main:
                    return mainPhase_;
                    break;
                case PhaseType::Post:
                    return postPhase_;
                    break;
            }

            std::unreachable();
        }


        /**
         * @brief Initializes the GameLoop and all registered phases and passes.
         **/
        void init() {

            assert(!initialized_ && "init() already called");

            ecsDataContainer_.borrow(gameWorld_.resourceRegistry());

            initialized_ = true;
        }

        GameWorld& gameWorld() noexcept {
            return gameWorld_;
        }

        /**
         * @brief Executes one full frame update across all phases.
         *
         * @param gameWorld Reference to the game world.
         * @param deltaTime Time elapsed since the last frame in seconds.
         * @param inputSnapshot Snapshot of the current input state.
         *
         */
        void update(const float deltaTime, const helios::engine::input::InputSnapshot& inputSnapshot
        ) noexcept {

            // clear the ContextProvider and previous ecsDataContainer
            ecsDataContainer_.clearOwned();

            assert(initialized_ && "GameLoop not initialized");

            totalTime_ += deltaTime;
            frameCount_++;

            auto updateContext = runtime::world::UpdateContext(
                  gameWorld_.session(),
                  gameWorld_.runtimeEnvironment(),
                  deltaTime,
                  totalTime_,
                  frameCount_,
                  inputSnapshot,
                  gameWorld_.ecsWorld()
            );

            ecsDataContainer_.emplace<UpdateContext>(updateContext);

            auto& session = gameWorld_.session();

            // gameloop phases
            prePhase_.update(ecsDataContainer_, gameWorld_.jobSystem());
            mainPhase_.update(ecsDataContainer_, gameWorld_.jobSystem());
            postPhase_.update(ecsDataContainer_, gameWorld_.jobSystem());
        }

        [[nodiscard]] bool isRunning() const noexcept {
            return initialized_ && !gameWorld_.session().isDestroyed();
        }

    };

}

