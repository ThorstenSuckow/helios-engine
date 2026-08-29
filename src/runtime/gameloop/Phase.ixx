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

import helios.core.thread.JobSystem;

import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.GameWorld;

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

        using EcsDataContainer =  ecs::common::container::EcsDataContainer;
        using JobSystem = helios::core::thread::JobSystem;

        /**
         * @brief Collection of passes belonging to this phase.
         */
        std::vector<std::unique_ptr<Pass>> passEntries_;

        GameWorld& gameWorld_;


        /**
         * @brief Updates all passes within this phase.
         *
         * @param updateContext The current update context.
         * @param ecsDataContainer The map of results from the current frame's system executions.
         * @param jobSystem The job system used for parallel execution of systems.
         */
        void update(ecs::common::container::EcsDataContainer& ecsDataContainer, common::Session& session, JobSystem& jobSystem){

            for (auto& pass : passEntries_) {

                if (pass->shouldRun(ecsDataContainer, session)) {
                    pass->update(ecsDataContainer, jobSystem);
                    pass->onPassEnd(ecsDataContainer);
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
        explicit Phase(GameWorld& gameWorld)
        : gameWorld_(gameWorld) {
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
         */
        template<typename StateType>
        Pass& beginPass(const StateType t) {
            auto entry = std::make_unique<TypedPass<StateType>>(*this, t, gameWorld_);
            auto* raw = entry.get();
            passEntries_.emplace_back(std::move(entry));

            return *raw;
        }


    };

}