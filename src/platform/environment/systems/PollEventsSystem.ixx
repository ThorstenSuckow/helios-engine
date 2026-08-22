/**
 * @file PollEventsSystem.ixx
 * @brief System that queues per-frame platform event polling commands.
 */
module;

export module helios.engine.platform.environment.systems.PollEventsSystem;



import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.types;
import helios.engine.runtime.concepts;
import helios.ecs.command;
import helios.ecs.system.tags;

import helios.engine.platform.environment.commands.PollEventsCommand;


using namespace helios::engine::runtime::world;
using namespace helios::ecs;
using namespace helios::ecs::command;
using namespace helios::engine::platform::environment::commands;
export namespace helios::engine::platform::environment::systems {

    /**
     * @brief Queues `PollEventsCommand` once per update call.
     */
    class PollEventsSystem {

        public:

        /**
         * @brief Engine role marker used by runtime system registries.
         */
        using EcsRoleTag = ecs::system::tags::TypedSystemRole;
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