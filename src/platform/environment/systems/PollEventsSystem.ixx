/**
 * @file PollEventsSystem.ixx
 * @brief System that queues per-frame platform event polling commands.
 */
module;

export module helios.engine.platform.environment.systems.PollEventsSystem;



import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.types;
import helios.engine.runtime.concepts;
import helios.ecs.command.NullCommandBuffer;
import helios.ecs.command.concepts;
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

        /**
         * @brief Enqueues polling of native platform/window events.
         *
         * @param updateCtx Frame-local update context.
         */
        template<typename TUpdateContextType, typename TCommandBuffer>
        requires runtime::concepts::ProvidesUpdateContext<TUpdateContextType, UpdateContext> &&
                 ecs::command::concepts::IsCommandBufferLike<TCommandBuffer>
        bool update(TUpdateContextType& updateCtx, TCommandBuffer& cmdBuffer) noexcept {
            (void)updateCtx.updateContext();
            cmdBuffer.template add<PollEventsCommand>();
            return true;
        }

    };



}