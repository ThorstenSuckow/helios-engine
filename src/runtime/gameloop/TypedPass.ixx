/**
 * @file TypedPass.ixx
 * @brief State-filtered pass implementation for game loop phases.
 */
module;

#include <type_traits>
#include <utility>
#include <cassert>

export module helios.engine.runtime.gameloop:TypedPass;

import :Pass;

import helios.engine.runtime.world.SystemRegistry;
import helios.engine.core.thread.JobSystem;
import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.Session;

import helios.engine.runtime.enginestate.types;

using namespace helios::engine::core::thread;
export namespace helios::engine::runtime::gameloop {

    class Phase;

    /**
     * @brief State-filtered pass that only executes in specific states.
     *
     * @details TypedPass extends the base Pass class with state-based filtering.
     * The pass only executes when the current state (queried from Session)
     * matches the configured state mask using bitwise AND.
     *
     * ## State Filtering
     *
     * The state mask is a bitfield where each bit represents a state value.
     * A pass runs if any bit in the mask matches the current state:
     *
     * ```cpp
     * // Run only in Running state
     * phase.beginPass<GameState>(GameState::Running)
     *     .addSystem<MovementSystem>();
     *
     * // Run in multiple states (bitwise OR)
     * phase.beginPass<GameState>(GameState::Running | GameState::Paused)
     *     .addSystem<InputSystem>();
     * ```
     *
     * @tparam StateType The state enum type (e.g., GameState, MatchState).
     *
     * @see Pass
     * @see Phase::beginPass()
     * @see Session::state()
     */
    template<typename StateType>
    class TypedPass : public Pass {

        friend class helios::engine::runtime::gameloop::Phase;

        /**
         * @brief Reference to the owning Phase.
         */
        Phase& owner_;

        /**
         * @brief Bitmask of states in which this pass should execute.
         */
        StateType mask_;

        /**
         * @brief Pointer to the job system used for parallel execution.
         */
        JobSystem* jobSystem_;

        /**
         * @brief Updates all systems registered in this pass.
         *
         * @param updateContext The current update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) override {

            assert(jobSystem_ && "Job system not initialized");

            for (auto typeIdQueue : systemTypeIdQueue_) {

                assert(typeIdQueue.size() > 0 && "Type ID queue is empty");

                if (typeIdQueue.size() == 1) {
                    systemRegistry_.item(typeIdQueue[0])->update(updateContext);
                    continue;
                }

                jobSystem_->runAndWait(
                    typeIdQueue.size(),
                    [&] (const std::size_t i) {
                        const auto typeId = typeIdQueue[i];
                        systemRegistry_.item(typeId)->update(updateContext);
                });

            }

        }


        /**
         * @brief Initializes all systems registered in this pass.
         *
         * @param gameWorld Reference to the game world.
         */
        void init(helios::engine::runtime::world::GameWorld& gameWorld) override {
            jobSystem_ = &gameWorld.jobSystem();
        }


        public:

        /**
         * @brief Constructs a typed pass for a state mask.
         *
         * @param owner Reference to the parent Phase.
         * @param mask State mask controlling when this pass runs.
         * @param gameWorld GameWorld used by the base Pass for buffer injection.
         */
        explicit TypedPass(
            Phase& owner, const StateType mask,
            helios::engine::runtime::world::GameWorld& gameWorld
        ) : owner_(owner), mask_(mask), Pass(gameWorld) {}

        /**
         * @copydoc Pass::endPass
         */
        Phase& endPass() override {
            return owner_;
        }


        /**
         * @brief Checks if this pass should execute based on current state.
         *
         * @details Queries the current state from the Session and compares it
         * against the configured mask using bitwise AND. The pass runs if
         * any bit in the mask matches the current state.
         *
         * @param updateContext The current update context.
         *
         * @return True if the pass should execute.
         */
        [[nodiscard]] bool shouldRun(helios::engine::runtime::world::UpdateContext& updateContext) const noexcept override {
            auto state = updateContext.session().state<StateType>();
            return hasFlag(mask_, state);
        }

        /**
         * @brief Checks if a value has any bit set in the mask.
         *
         * @param mask The bitmask to check against.
         * @param value The value to test.
         *
         * @return True if any bit in mask matches value.
         */
        bool hasFlag(StateType mask, StateType value) const noexcept {
            using U = std::underlying_type_t<StateType>;
            return (static_cast<U>(mask) & static_cast<U>(value)) != 0;
        }


    };

}