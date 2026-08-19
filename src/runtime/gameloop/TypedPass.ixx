/**
 * @file TypedPass.ixx
 * @brief State-filtered pass implementation for game loop phases.
 */
module;

#include <type_traits>
#include <utility>
#include <cassert>
#include <functional>
#include "helios-engine-config.h"


export module helios.engine.runtime.gameloop:TypedPass;

import :Pass;

import helios.core.thread.JobSystem;
import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.Session;

import helios.engine.runtime.enginestate.types;

using namespace helios::core::thread;
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

        using RunCondition = typename Pass::RunCondition;

        std::vector<RunCondition> runConditions_;

        using Pass::systemRegistry_;
        using Pass::systemTypeIdQueue_;
        using Pass::managerTypeIds;
        using Pass::parallelManagerTypeIds;
        using Pass::gameWorld_;
        using Pass::contextProvider_;

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
                        auto* system = systemRegistry_.item(serialSystem);
                        // update, then immediately flush the buffer contents
                        const auto updateTypeId = system->expectedUpdateContextTypeId();
                        auto updateCtx = contextProvider_.get(updateTypeId, updateContext);
                        system->update(updateCtx);

                        if (auto* commandBuffer = system->commandBuffer()) {
                            const auto flushTypeId = commandBuffer->expectedFlushContextTypeId();
                            auto ctx = contextProvider_.get(flushTypeId);
                            commandBuffer->flush(ctx);
                        }
                    }
                    continue;
                }

                // parallelSystems > 1 will be queued with the JobSystems
                jobSystem_->runAndWait(
                    parallelSystems.size(),
                    [&] (const std::size_t i) {
                        // a parallel system owns more ore more serial systems
                        for (const auto& serialSystem : parallelSystems[i]) {
                            auto* system = systemRegistry_.item(serialSystem);

                            const auto updateTypeId = system->expectedUpdateContextTypeId();
                            auto updateCtx = contextProvider_.get(updateTypeId, updateContext);
                            system->update(updateCtx);
                        }
                });

                // once parallel systems where updates, flush their command buffers
                for (const auto& parallelSystem : parallelSystems) {
                    for (const auto& serialSystem : parallelSystem) {
                        auto* system = systemRegistry_.item(serialSystem);

                        if (auto* commandBuffer = system->commandBuffer()) {
                            const auto flushTypeId = commandBuffer->expectedFlushContextTypeId();
                            auto ctx = contextProvider_.get(flushTypeId);
                            commandBuffer->flush(ctx);
                        }
                    }
                }

            }

        }

        void onPassEnd(runtime::world::UpdateContext& updateContext) noexcept override {

            auto exec = [&, this](auto& typeIds, const bool isParallel = false) {

                for (const auto typeId : typeIds) {
                    auto* manager = gameWorld_.managerRegistry().item(typeId);
                    #if HELIOS_DEBUG
                    if (!manager) {
                        assert(manager && "Manager not found in registry");
                    }
                    #endif

                    auto ctxTypeId = manager->expectedExecutionContextTypeId();
                    auto contextRef = contextProvider_.get(ctxTypeId, updateContext);

                    if (isParallel) {
                        manager->executeCommandsParallel(contextRef);
                    } else {
                        manager->executeCommands(contextRef);
                    }

                    if (auto* commandBuffer = manager->commandBuffer()) {
                        const auto flushCtxTypeId = commandBuffer->expectedFlushContextTypeId();
                        auto flushContextRef = contextProvider_.get(flushCtxTypeId);
                        commandBuffer->flush(flushContextRef);
                    }
                }
            };

            exec(managerTypeIds_, false);
            exec(parallelManagerTypeIds_,true);
        }


        /**
         * @brief Initializes all systems registered in this pass.
         *
         * @param gameWorld Reference to the game world.
         */
        void init() override {
            jobSystem_ = &gameWorld_.jobSystem();

            for (auto* system : systemRegistry_.items()) {
                if (auto* commandBuffer = system->commandBuffer()) {
                    const auto flushTypeId = commandBuffer->expectedFlushContextTypeId();
                    auto ctx = contextProvider_.get(flushTypeId);
                    commandBuffer->flush(ctx);
                }
            }
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
            helios::engine::runtime::world::GameWorld& gameWorld,
            helios::engine::runtime::world::ContextProvider& contextProvider
        ) : owner_(owner), mask_(mask), Pass(gameWorld, contextProvider) {}

        /**
         * @copydoc Pass::endPass
         */
        Phase& endPass() override {
            return owner_;
        }

        /**
         * @copydoc Pass::runIf
         */
        Pass& runIf(RunCondition fn) noexcept override {
            runConditions_.push_back(std::move(fn));
            return *this;
        }




    };

}