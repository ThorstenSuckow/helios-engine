/**
 * @file PollEventsSystem.ixx
 * @brief System that queues per-frame platform event polling commands.
 */
module;

export module helios.engine.platform.environment.systems.PollEventsSystem;



import helios.engine.runtime.gameloop.types;
import helios.ecs.command;


import helios.engine.platform.environment.commands.PollEventsCommand;



using namespace helios::ecs;
using namespace helios::ecs::command;
using namespace helios::engine::platform::environment::commands;
export namespace helios::engine::platform::environment::systems {

    /**
     * @brief Queues `PollEventsCommand` once per update call.
     */
    class PollEventsSystem {

        using UpdateContext = engine::runtime::gameloop::types::UpdateContext;

        public:

        using CommandBuffer = ecs::command::TypedCommandBuffer<PollEventsCommand>;

        /**
         * @brief Enqueues polling of native platform/window events.
         *
         * @param updateContext Frame-local update context.
         */
        void update(UpdateContext& updateContext, CommandBuffer& cmdBuffer) noexcept {
            (void)updateContext;
            cmdBuffer.template add<PollEventsCommand>();
        }

    };



}