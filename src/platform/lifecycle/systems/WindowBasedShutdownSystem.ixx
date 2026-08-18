/**
 * @file WindowBasedShutdownSystem.ixx
 * @brief System that requests runtime shutdown when no active windows remain.
 */
module;

export module helios.engine.platform.lifecycle.systems.WindowBasedShutdownSystem;



import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.types;
import helios.engine.runtime.concepts;

import helios.ecs.system.tags;

import helios.engine.runtime.world;
import helios.ecs;

import helios.engine.platform.window.components;
import helios.engine.platform.window.concepts;

import helios.engine.platform.lifecycle.commands;
import helios.engine.platform.environment.components;
import helios.engine.platform.environment.concepts;


using namespace helios::engine::runtime::world;
using namespace helios::ecs;
using namespace helios::ecs::common::concepts;
using namespace helios::engine::platform::environment::components;
using namespace helios::engine::platform::window::components;
using namespace helios::engine::platform::window::concepts;
using namespace helios::engine::platform::environment::concepts;
using namespace helios::engine::platform::lifecycle::commands;
using namespace helios::ecs::components;

export namespace helios::engine::platform::lifecycle::systems {

    /**
     * @brief Queues `ShutdownCommand` when no active window entities are left.
     *
     * @tparam THandle Window handle type.
     * @tparam TUpdateContextType Type of the UpdateContext to use.
     */
    template<typename THandle,
             typename TCommandBuffer = ecs::command::NullCommandBuffer,
             typename TUpdateContextType = types::SystemUpdateContext>
    requires IsWindowHandle<THandle> &&
             ecs::command::concepts::IsCommandBufferLike<TCommandBuffer> &&
             runtime::concepts::ProvidesUpdateContext<TUpdateContextType, UpdateContext>
    class WindowBasedShutdownSystem {

    public:

        using CommandBuffer_type = TCommandBuffer;
        using UpdateContextType = TUpdateContextType;

        /**
         * @brief Engine role marker used by runtime registries.
         */
        using EcsRoleTag = ecs::system::tags::TypedSystemRole;

        /**
         * @brief Checks window activity and queues shutdown when the set is empty.
         *
         * @param updateCtx Frame-local update context.
         * @param cmdBuffer Command buffer for submitting shutdown commands.
         */
        bool update(TUpdateContextType& updateCtx, TCommandBuffer& cmdBuffer) noexcept {

            auto& updateContext = updateCtx.updateContext();

            if (updateContext.template view<THandle, WindowComponent<THandle>>().withActive().empty()) {
               cmdBuffer.template add<ShutdownCommand>();
            }


            return true;
        }

    };



}