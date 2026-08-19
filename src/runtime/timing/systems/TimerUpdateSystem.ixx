/**
 * @file TimerUpdateSystem.ixx
 * @brief System that advances all game timers each frame.
 */
module;

#include <span>

export module helios.engine.runtime.timing.systems.TimerUpdateSystem;

import helios.engine.runtime.timing.Timer;
import helios.engine.runtime.timing.TimerManager;


import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.types;
import helios.engine.runtime.concepts;
import helios.ecs.command;
import helios.ecs.common.concepts;

import helios.ecs.system.tags;

import helios.engine.runtime.timing.types;
import helios.engine.runtime.timing.commands;

using namespace helios::engine::runtime::timing;

using namespace helios::engine::runtime::timing::types;
using namespace helios::engine::runtime::timing::commands;
using namespace helios::engine::runtime::world;
using namespace helios::ecs;
using namespace helios::ecs::common::concepts;

export namespace helios::engine::runtime::timing::systems {

    /**
     * @brief Updates all game timers managed by the TimerManager.
     *
     * Each frame, this system iterates over all registered timers and
     * advances their elapsed time by the current delta time.
     *
     * @see TimerManager
     * @see Timer
     */
    template<typename TTimerManager>
    class TimerUpdateSystem {

        /**
         * @brief Reference to the TimerManager owning the timers.
         */
        TTimerManager& timerManager_;

    public:


        using EcsRoleTag = ecs::system::tags::TypedSystemRole;


        /**
         * @brief Constructs the system with a reference to the TimerManager.
         *
         * @param timerManager The manager whose timers are updated.
         */
        explicit TimerUpdateSystem(TTimerManager& timerManager)
        : timerManager_(timerManager) {}

        /**
         * @brief Advances all game timers by the current delta time.
         *
         * @param updateCtx The current frame's update context.
         */
        template<typename TUpdateContextType, typename TCommandBuffer>
        requires runtime::concepts::ProvidesUpdateContext<TUpdateContextType, helios::engine::runtime::world::UpdateContext> &&
                 ecs::command::concepts::IsCommandBufferLike<TCommandBuffer>
        bool update(TUpdateContextType& updateCtx, TCommandBuffer& cmdBuffer) noexcept {

            auto& updateContext = updateCtx.updateContext();

            for (auto& timer : timerManager_.timers()) {
                if (timer.state() == TimerState::Running) {

                    timer.update(updateContext.deltaTime());

                    if (timer.duration() != 0.0f && timer.elapsed() >= timer.duration()) {
                        auto context = TimerControlContext{timer.timerId(), TimerState::Finished};
                        cmdBuffer.template add<TimerControlCommand>(context);
                    }
                }
            }
            return true;
        }

    };

}
