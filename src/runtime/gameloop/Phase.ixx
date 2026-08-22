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
import helios.engine.runtime.world.ContextProvider;

import helios.engine.runtime.world.Session;


import helios.engine.runtime.enginestate.types;

using namespace helios::engine::runtime::enginestate::types;
using namespace helios::engine::runtime::world;

export namespace helios::engine::runtime::gameloop {

    class GameLoop;


    /**
     * @brief Enumeration of game loop phase types.
     */
    enum class PhaseType {
        Pre,
        Main,
        Post
    };

    /**
     * @brief Represents a phase in the game loop containing multiple passes.
     */
    class Phase {

        friend class helios::engine::runtime::gameloop::GameLoop;

        /**
         * @brief Collection of passes belonging to this phase.
         */
        std::vector<std::unique_ptr<Pass>> passEntries_;

        GameWorld& gameWorld_;

        ContextProvider& contextProvider_;

        /**
         * @brief Initializes all passes within this phase.
         */
        void init(){
            for (auto& pass : passEntries_) {
                // every pass contains systems that are updated here
                pass->init();
            }
        };

        /**
         * @brief Updates all passes within this phase.
         *
         * @param updateContext The current update context.
         * @param ecsDataContainer The map of results from the current frame's system executions.
         */
        void update(runtime::world::UpdateContext& updateContext,
            ecs::common::container::EcsDataContainer& ecsDataContainer){

            for (auto& pass : passEntries_) {

                if (pass->shouldRun(updateContext)) {
                    pass->update(updateContext, ecsDataContainer);;
                    pass->onPassEnd(updateContext, ecsDataContainer);
                }
            }
        };


    public:

        /**
         * @brief Constructs a Phase with references to GameLoop and GameWorld.
         *
         * @param gameloop Reference to the parent GameLoop.
         * @param gameWorld Shared GameWorld used by passes in this phase.
         */
        explicit Phase(GameWorld& gameWorld, ContextProvider& contextProvider)
        : gameWorld_(gameWorld), contextProvider_(contextProvider) {
        }


        /**
         * @brief Creates and adds a new typed pass to this phase.
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
            auto entry = std::make_unique<TypedPass<StateType>>(*this, t, gameWorld_, contextProvider_);
            auto* raw = entry.get();
            passEntries_.emplace_back(std::move(entry));

            return *raw;
        }


    };

}