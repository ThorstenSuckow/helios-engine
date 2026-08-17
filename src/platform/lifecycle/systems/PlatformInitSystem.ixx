/**
 * @file PlatformInitSystem.ixx
 * @brief System that requests runtime platform initialization while session is not initialized.
 */
module;

export module helios.engine.platform.lifecycle.systems.PlatformInitSystem;



import helios.engine.runtime.world.UpdateContext;

import helios.engine.runtime.world.tags.SystemRole;

import helios.engine.runtime.world;
import helios.ecs;

import helios.engine.platform.lifecycle.commands;


using namespace helios::engine::runtime::world::tags;
using namespace helios::engine::runtime::world;
using namespace helios::ecs;
using namespace helios::ecs::common::concepts;
using namespace helios::engine::platform::lifecycle::commands;
export namespace helios::engine::platform::lifecycle::systems {

    /**
     * @brief Submits `PlatformInitCommand` until session/runtime initialization completed.
     *
     * @tparam TCommandBuffer Command buffer used for platform init command submission.
     */
    template<typename TCommandBuffer = ecs::command::NullCommandBuffer>
    requires ecs::command::concepts::IsCommandBufferLike<TCommandBuffer>
    class PlatformInitSystem {

        public:

        using CommandBuffer_type = TCommandBuffer;

        /**
         * @brief Engine role marker used by runtime system registries.
         */
        using EcsRoleTag = TypedSystemRole;

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