/**
 * @file PlatformInitSystem.ixx
 * @brief System that requests runtime platform initialization while session is not initialized.
 */
module;

export module helios.engine.platform.lifecycle.systems.PlatformInitSystem;



import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.types;
import helios.engine.runtime.concepts;

import helios.ecs.system.tags;

import helios.engine.runtime.world;
import helios.ecs;

import helios.engine.platform.lifecycle.commands;



using namespace helios::engine::runtime::world;
using namespace helios::ecs;
using namespace helios::ecs::common::concepts;
using namespace helios::engine::platform::lifecycle::commands;
export namespace helios::engine::platform::lifecycle::systems {

    /**
     * @brief Submits `PlatformInitCommand` until session/runtime initialization completed.
     *
     * @tparam TCommandBuffer Command buffer used for platform init command submission.
     * @tparam TUpdateContextType Type of the UpdateContext to use.
     */
    template<
        typename TCommandBuffer = ecs::command::NullCommandBuffer,
        typename TUpdateContextType = types::SystemUpdateContext
    >
    requires ecs::command::concepts::IsCommandBufferLike<TCommandBuffer> &&
             runtime::concepts::ProvidesUpdateContext<TUpdateContextType, UpdateContext>
    class PlatformInitSystem {

        public:

        using CommandBuffer_type = TCommandBuffer;
        using UpdateContextType = TUpdateContextType;

        /**
         * @brief Engine role marker used by runtime system registries.
         */
        using EcsRoleTag = ecs::system::tags::TypedSystemRole;

        /**
         * @brief Queues platform initialization command when required.
         *
         * @param updateCtx Frame-local update context.
         */
        bool update(TUpdateContextType& updateCtx, TCommandBuffer& cmdBuffer) noexcept {

            auto& updateContext = updateCtx.updateContext();

            if (!updateContext.session().isInitialized()) {
                 cmdBuffer.template add<PlatformInitCommand>();
            }
            return true;
        }

    };



}