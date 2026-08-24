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
     * @brief State-filtered pass that only executes in specific states and if arbitrary conditions are satisifed.
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
         * @brief List of run conditions that must be satisfied for this pass to be executed.
         */
        std::vector<RunCondition> runConditions_{};

        using Pass::systemRegistry_;
        using Pass::systemTypeIdQueue_;
        using Pass::managerTypeIds;
        using Pass::gameWorld_;
        using EcsDataContainer = ecs::common::container::EcsDataContainer;
        using JobSystem = helios::core::thread::JobSystem;

    protected:
        /**
         * @copydoc Pass::update
         */
        void update(EcsDataContainer& ecsDataContainer, JobSystem& jobSystem) override {


            for (auto& parallelSystems : systemTypeIdQueue_) {

                // parallelSystems with only one entry are treated serial
                if (parallelSystems.size() == 1) {
                    for (const auto& serialSystem : parallelSystems[0]) {
                        auto* system = systemRegistry_.item(serialSystem);
                        // update, then immediately flush the buffer contents
                        // consume frame results
                        system->update(ecsDataContainer);

                        // produce frame results
                        system->flush(ecsDataContainer);

                        if (auto* commandBuffer = system->commandBuffer()) {
                            commandBuffer->flush(ecsDataContainer);
                        }
                    }
                    continue;
                }

                // parallelSystems > 1 will be queued with the JobSystems
                jobSystem.runAndWait(
                    parallelSystems.size(),
                    [&] (const std::size_t i) {
                        // a parallel system owns more ore more serial systems
                        for (const auto& serialSystem : parallelSystems[i]) {
                            auto* system = systemRegistry_.item(serialSystem);
                            system->update(ecsDataContainer);
                        }
                });

                // once parallel systems where updates, flush their command buffers
                for (const auto& parallelSystem : parallelSystems) {
                    for (const auto& serialSystem : parallelSystem) {
                        auto* system = systemRegistry_.item(serialSystem);
                        system->flush(ecsDataContainer);

                        if (auto* commandBuffer = system->commandBuffer()) {
                            commandBuffer->flush(ecsDataContainer);
                        }
                    }
                }

            }

        }

        /**
         * @copydoc Pass::onPassEnd
         */
        void onPassEnd(EcsDataContainer& ecsDataContainer) noexcept override {

            for (const auto typeId : managerTypeIds_) {
                auto* manager = gameWorld_.managerRegistry().item(typeId);
                #if HELIOS_DEBUG
                if (!manager) {
                    assert(manager && "Manager not found in registry");
                }
                #endif

                manager->executeCommands(ecsDataContainer);

                if (auto* commandBuffer = manager->commandBuffer()) {
                    commandBuffer->flush(ecsDataContainer);
                }
            }
        }

       /**
        * @copydoc Pass::shouldRun
        */
        [[nodiscard]] bool shouldRun(EcsDataContainer& ecsDataContainer) const noexcept override {
            auto& updateContext = ecsDataContainer.get<UpdateContext>();
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
        explicit TypedPass(Phase& owner, const StateType mask, GameWorld& gameWorld)
        : owner_(owner), mask_(mask), Pass(gameWorld) {}

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