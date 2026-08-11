/**
 * @file TypedPass.ixx
 * @brief State-filtered pass implementation for game loop phases.
 */
module;

#include <type_traits>
#include <utility>
#include <cassert>
#include <functional>

export module helios.engine.runtime.gameloop:TypedPass;

import :Pass;

import helios.engine.runtime.world.SystemRegistry;
import helios.core.thread.JobSystem;
import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.Session;

import helios.engine.runtime.enginestate.types;

using namespace helios::core::thread;
export namespace helios::engine::runtime::gameloop {

    template<typename TTypedHandleWorld>
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
    template<typename StateType, typename TTypedHandleWorld>
    class TypedPass : public Pass<TTypedHandleWorld> {

        friend class helios::engine::runtime::gameloop::Phase<TTypedHandleWorld>;

        /**
         * @brief Reference to the owning Phase.
         */
        Phase<TTypedHandleWorld>& owner_;

        /**
         * @brief Bitmask of states in which this pass should execute.
         */
        StateType mask_;

        /**
         * @brief Pointer to the job system used for parallel execution.
         */
        JobSystem* jobSystem_;

        using Pass<TTypedHandleWorld>::systemRegistry_;
        using Pass<TTypedHandleWorld>::systemTypeIdQueue_;
        using Pass<TTypedHandleWorld>::managerTypeIds;
        using Pass<TTypedHandleWorld>::parallelManagerTypeIds;
        using Pass<TTypedHandleWorld>::gameWorld_;

        /**
         * @brief Updates all systems registered in this pass.
         *
         * @param updateContext The current update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) override {

            assert(jobSystem_ && "Job system not initialized");

            for (auto& parallelSystems : systemTypeIdQueue_) {

                // parallelSystems with only one entry are treated serial
                if (parallelSystems.size() == 1) {
                    for (const auto& serialSystem : parallelSystems[0]) {
                        auto* sys = systemRegistry_.item(serialSystem);
                        // update, then immediately flush the buffer contents
                        sys->update(updateContext);
                        sys->flush(updateContext);
                    }
                    continue;
                }

                // parallelSystems > 1 will be queued with the JobSystems
                jobSystem_->runAndWait(
                    parallelSystems.size(),
                    [&] (const std::size_t i) {
                        // a parallel system owns more ore more serial systems
                        for (const auto& serialSystem : parallelSystems[i]) {
                            auto* sys = systemRegistry_.item(serialSystem);
                            sys->update(updateContext);
                        }
                });

                for (const auto& parallelSystem : parallelSystems) {
                    for (const auto& serialSystem : parallelSystem) {
                        auto* sys = systemRegistry_.item(serialSystem);
                        sys->flush(updateContext);
                    }
                }

            }

        }

        void onPassEnd(runtime::world::UpdateContext& updateContext) noexcept override {

            for (const auto typeId : managerTypeIds()) {
                gameWorld_.managerRegistry().item(typeId)->flush(updateContext);
            }

            for (const auto typeId : parallelManagerTypeIds()) {
                gameWorld_.managerRegistry().item(typeId)->flushParallel(updateContext);
            }
        }

        /**
         * @brief Initializes all systems registered in this pass.
         *
         * @param gameWorld Reference to the game world.
         */
        void init() override {
            jobSystem_ = &gameWorld_.jobSystem();

            for (auto* system : systemRegistry_.items()) {
                system->init(gameWorld_);
            }
        }

        using RunCondition = typename Pass<TTypedHandleWorld>::RunCondition;

        std::vector<RunCondition> runConditions_;

        public:

        /**
         * @brief Constructs a typed pass for a state mask.
         *
         * @param owner Reference to the parent Phase.
         * @param mask State mask controlling when this pass runs.
         * @param gameWorld GameWorld used by the base Pass for buffer injection.
         */
        explicit TypedPass(
            Phase<TTypedHandleWorld>& owner, const StateType mask,
            helios::engine::runtime::world::GameWorld<TTypedHandleWorld>& gameWorld
        ) : owner_(owner), mask_(mask), Pass<TTypedHandleWorld>(gameWorld) {}

        /**
         * @copydoc Pass::endPass
         */
        Phase<TTypedHandleWorld>& endPass() override {
            return owner_;
        }

        /**
         * @copydoc Pass::runIf
         */
        Pass<TTypedHandleWorld>& runIf(RunCondition fn) noexcept override {
            runConditions_.push_back(std::move(fn));
            return *this;
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
            if (!hasFlag(mask_, state)) {
                return false;
            }

            for (const auto& condition : runConditions_) {
                if (!condition(updateContext)) {
                    return false;
                }
            }

            return true;
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