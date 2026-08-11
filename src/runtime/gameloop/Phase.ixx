/**
 * @file Phase.ixx
 * @brief Represents a phase within the game loop containing multiple passes.
 */
module;

#include <memory>
#include <vector>

export module helios.engine.runtime.gameloop:Phase;

import :Pass;
import :TypedPass;

import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.GameWorld;

import helios.engine.runtime.world.Session;


import helios.engine.runtime.enginestate.types;

using namespace helios::engine::runtime::enginestate::types;
using namespace helios::engine::runtime::world;

export namespace helios::engine::runtime::gameloop {

    template<typename TTypedHandleWorld>
    class GameLoop;


    /**
     * @brief Enumeration of game loop phase types.
     *
     * The game loop is divided into three sequential phases:
     * - **Pre:** Input processing and command generation.
     * - **Main:** Core gameplay simulation (physics, AI, game logic).
     * - **Post:** Synchronization, cleanup, and preparation for rendering.
     */
    enum class PhaseType {
        /**
         * @brief Pre-update phase for input and command processing.
         */
        Pre,

        /**
         * @brief Main update phase for core gameplay systems.
         */
        Main,

        /**
         * @brief Post-update phase for cleanup and scene synchronization.
         */
        Post
    };

    /**
     * @brief Represents a phase in the game loop containing multiple passes.
     *
     * @see Pass
     * @see GameLoop
     */
    template<typename TTypedHandleWorld>
    class Phase {

        friend class helios::engine::runtime::gameloop::GameLoop<TTypedHandleWorld>;


        /**
         * @brief Initializes all passes within this phase.
         *
         * @param gameWorld Reference to the game world.
         */
        void init(GameWorld<TTypedHandleWorld>& gameWorld){
            for (auto& pass : passEntries_) {
                // every pass contains systems that are updated here
                pass->init();
            }
        };

        /**
         * @brief Updates all passes within this phase.
         *
         * @param gameWorld The game world where the update occurred.
         * @param updateContext The current update context.
         *
         * @see CommitPoint
         * @see Pass::addCommitPoint()
         * @see Pass::runsIn()
         */
        void update(GameWorld<TTypedHandleWorld>& gameWorld, runtime::world::UpdateContext& updateContext){

            for (auto& pass : passEntries_) {

                if (pass->shouldRun(updateContext)) {
                    pass->update(updateContext);
                    pass->onPassEnd(updateContext);
                }

            }
        };

        /**
         * @brief Collection of passes belonging to this phase.
         */
        std::vector<std::unique_ptr<Pass<TTypedHandleWorld>>> passEntries_;

        /**
         * @brief Reference to the owning GameLoop.
         */
        helios::engine::runtime::gameloop::GameLoop<TTypedHandleWorld>& gameloop_;

        helios::engine::runtime::world::GameWorld<TTypedHandleWorld>& gameWorld_;

    public:

        /**
         * @brief Constructs a Phase with references to GameLoop and GameWorld.
         *
         * @param gameloop Reference to the parent GameLoop.
         * @param gameWorld Shared GameWorld used by passes in this phase.
         */
        explicit Phase(helios::engine::runtime::gameloop::GameLoop<TTypedHandleWorld>& gameloop, GameWorld<TTypedHandleWorld>& gameWorld) : gameloop_(gameloop), gameWorld_(gameWorld) {

        }


        /**
         * @brief Creates and adds a new typed pass to this phase.
         *
         * @details The state parameter specifies in which states this pass
         * should execute. Passes are skipped if the current state does not
         * match the configured mask (using bitwise AND). New passes are
         * bound to this phase's GameWorld reference.
         *
         * @tparam StateType The state enum type (e.g., GameState, MatchState).
         *
         * @param t The state mask specifying when this pass should run.
         *
         * @return Reference to the newly created Pass for method chaining.
         *
         * @see TypedPass
         * @see Session::state()
         */
        template<typename StateType>
        Pass<TTypedHandleWorld>& beginPass(const StateType t) {
            auto entry = std::make_unique<TypedPass<StateType, TTypedHandleWorld>>(*this, t, gameWorld_);
            auto* raw = entry.get();
            passEntries_.emplace_back(std::move(entry));

            return *raw;
        }


    };

}