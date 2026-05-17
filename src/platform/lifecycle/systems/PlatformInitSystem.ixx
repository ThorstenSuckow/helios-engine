/**
 * @file PlatformInitSystem.ixx
 * @brief System that requests runtime platform initialization while session is not initialized.
 */
module;

export module helios.engine.platform.lifecycle.systems.PlatformInitSystem;



import helios.engine.runtime.world.UpdateContext;

import helios.engine.runtime.world.tags.SystemRole;

import helios.engine.runtime.world;
import helios.engine.runtime.messaging.command.NullCommandBuffer;
import helios.engine.runtime.messaging.command.concepts.IsCommandBufferLike;

import helios.engine.platform.lifecycle.commands;

import helios.ecs.components.Active;

using namespace helios::engine::runtime::world::tags;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::messaging::command;
using namespace helios::engine::runtime::messaging::command::concepts;
using namespace helios::engine::platform::lifecycle::commands;
export namespace helios::engine::platform::lifecycle::systems {

    /**
     * @brief Submits `PlatformInitCommand` until session/runtime initialization completed.
     *
     * @tparam TCommandBuffer Command buffer used for platform init command submission.
     */
    template<typename TCommandBuffer = NullCommandBuffer>
    requires IsCommandBufferLike<TCommandBuffer>
    class PlatformInitSystem {

        public:

        using CommandBuffer_type = TCommandBuffer;

        /**
         * @brief Engine role marker used by runtime system registries.
         */
        using EngineRoleTag = SystemRole;

        /**
         * @brief Queues platform initialization command when required.
         *
         * @param updateContext Frame-local update context.
         */
        void update(UpdateContext& updateContext, TCommandBuffer& cmdBuffer) noexcept {

            if (!updateContext.session().isInitialized()) {
                 cmdBuffer.template add<PlatformInitCommand>();
            }
        }

    };



}