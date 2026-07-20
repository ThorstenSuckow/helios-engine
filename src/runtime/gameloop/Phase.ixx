/**
 * @file Phase.ixx
 * @brief Represents a phase within the game loop containing multiple passes.
 */
module;

#include <memory>
#include <vector>

export module helios.engine.runtime.gameloop:Phase;

import :PassEndListener;
import :Pass;
import :TypedPass;

import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.GameWorld;

import helios.engine.runtime.world.Session;


import helios.engine.runtime.enginestate.types;

using namespace helios::engine::runtime::enginestate::types;
using namespace helios::engine::runtime::world;

export namespace helios::engine::runtime::gameloop {
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
    class Phase {

        friend class helios::engine::runtime::gameloop::GameLoop;

        /**
         * @brief Collection of listeners to be notified when a pass ends.
         *
         * @see PassEndListener
         * @see notifyPassEndListeners()
         */
        std::vector<PassEndListener*> passEndListeners_;

        /**
         * @brief Initializes all passes within this phase.
         *
         * @param gameWorld Reference to the game world.
         */
        void init(GameWorld& gameWorld){
            for (auto& pass : passEntries_) {
                // every pass contains systems that are updated here
                pass->init(gameWorld);
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
        void update(GameWorld& gameWorld, UpdateContext& updateContext){

            for (auto& pass : passEntries_) {

                if (pass->shouldRun(updateContext)) {
                    pass->update(updateContext);
                    notifyPassEndListeners(*pass, gameWorld, updateContext);
                }

            }
        };

        /**
         * @brief Notifies all registered listeners about a pass reaching its end.
         *
         * @param pass The pass that reached its end.
         * @param gameWorld The game world where the pass end occured.
         * @param updateContext The current update context.
         *
         * @return Always returns true.
         *
         * @see PassEndListener::onPassCommit()
         * @see addPassEndListener()
         */
        bool notifyPassEndListeners(Pass& pass, GameWorld& gameWorld, UpdateContext& updateContext) {

            for (const auto& passEndListener : passEndListeners_) {
                passEndListener->onPassEnd(pass, gameWorld, updateContext);
            }
            return true;
        }

        /**
         * @brief Collection of passes belonging to this phase.
         */
        std::vector<std::unique_ptr<Pass>> passEntries_;

        /**
         * @brief Reference to the owning GameLoop.
         */
        helios::engine::runtime::gameloop::GameLoop& gameloop_;

        helios::engine::runtime::world::GameWorld& gameWorld_;

    public:

        /**
         * @brief Constructs a Phase with references to GameLoop and GameWorld.
         *
         * @param gameloop Reference to the parent GameLoop.
         * @param gameWorld Shared GameWorld used by passes in this phase.
         */
        explicit Phase(helios::engine::runtime::gameloop::GameLoop& gameloop, GameWorld& gameWorld) : gameloop_(gameloop), gameWorld_(gameWorld) {

        }


        /**
         * @brief Registers a listener to be notified when passes end.
         *
         * @param passEndListener Pointer to the listener to register. Must remain
         *        valid for the lifetime of this Phase or until removed.
         *
         * @return True if the listener was added, false if it was already registered.
         *
         * @see PassEndListener
         * @see notifyPassEndListeners()
         */
        bool addPassEndListener(PassEndListener* passEndListener) {

            for (int i = 0; i < passEndListeners_.size(); i++) {
                if (passEndListeners_[i] == passEndListener) {
                    return false;
                }
            }

            passEndListeners_.emplace_back(passEndListener);

            return true;
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
        Pass& beginPass(const StateType t) {
            auto entry = std::make_unique<TypedPass<StateType>>(*this, t, gameWorld_);
            auto* raw = entry.get();
            passEntries_.emplace_back(std::move(entry));

            return *raw;
        }

        /**
         * @brief Returns a reference to the owning GameLoop.
         *
         * @return Reference to the parent GameLoop.
         */
        [[nodiscard]] helios::engine::runtime::gameloop::GameLoop& gameLoop() noexcept {
            return gameloop_;
        }


    };

}