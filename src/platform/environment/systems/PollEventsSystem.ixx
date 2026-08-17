/**
 * @file PollEventsSystem.ixx
 * @brief System that queues per-frame platform event polling commands.
 */
module;

export module helios.engine.platform.environment.systems.PollEventsSystem;



import helios.engine.runtime.world.UpdateContext;
import helios.ecs.command.NullCommandBuffer;
import helios.ecs.command.concepts;
import helios.engine.runtime.world.tags.SystemRole;

import helios.engine.platform.environment.commands.PollEventsCommand;

using namespace helios::engine::runtime::world::tags;
using namespace helios::engine::runtime::world;
using namespace helios::ecs;
using namespace helios::ecs::command;
using namespace helios::engine::platform::environment::commands;
export namespace helios::engine::platform::environment::systems {

    /**
     * @brief Queues `PollEventsCommand` once per update call.
     */
    template<typename TCommandBuffer = ecs::command::NullCommandBuffer>
    requires ecs::command::concepts::IsCommandBufferLike<TCommandBuffer>
    class PollEventsSystem {

        public:

        using CommandBuffer_type = TCommandBuffer;

        /**
         * @brief Engine role marker used by runtime system registries.
         */
        using EcsRoleTag = TypedSystemRole;

        /**
         * @brief Enqueues polling of native platform/window events.
         *
         * @param updateContext Frame-local update context.
         */
        void update(UpdateContext& updateContext, TCommandBuffer& cmdBuffer) noexcept {
            cmdBuffer.template add<PollEventsCommand>();
        }

    };



}