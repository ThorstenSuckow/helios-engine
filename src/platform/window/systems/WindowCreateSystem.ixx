/**
 * @file WindowCreateSystem.ixx
 * @brief System that translates window-create request components into window-create commands.
 */
module;

export module helios.engine.platform.window.systems.WindowCreateSystem;



import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.types;
import helios.engine.runtime.concepts;
import helios.ecs.command.NullCommandBuffer;
import helios.ecs.command.concepts;

import helios.ecs.system.tags;

import helios.engine.platform.window.components.WindowCreateRequestComponent;
import helios.engine.platform.window.commands.WindowCreateCommand;

import helios.ecs.component;
import helios.engine.platform.window.concepts.IsWindowHandle;

using namespace helios::engine::platform::window::concepts;

using namespace helios::engine::runtime::world;
using namespace helios::ecs::command::concepts;
using namespace helios::ecs;
using namespace helios::engine::platform::window::components;
using namespace helios::engine::platform::window::commands;
using namespace helios::ecs::components;
using namespace helios::engine::platform::window::components;
export namespace helios::engine::platform::window::systems {

    /**
     * @brief Queues `WindowCreateCommand` for active entities with pending create requests.
     *
     * @tparam THandle Window-domain entity handle type.
     * @tparam TUpdateContextType Type of the UpdateContext to use.
     */
    template<typename THandle,
             typename TCommandBuffer = ecs::command::NullCommandBuffer,
             typename TUpdateContextType = types::SystemUpdateContext>
    requires IsWindowHandle<THandle> &&
             ecs::command::concepts::IsCommandBufferLike<TCommandBuffer> &&
             runtime::concepts::ProvidesUpdateContext<TUpdateContextType, UpdateContext>
    class WindowCreateSystem {

        public:

        using CommandBuffer_type = TCommandBuffer;
        using UpdateContextType = TUpdateContextType;

        /**
         * @brief Engine role marker used by runtime registries.
         */
        using EcsRoleTag = ecs::system::tags::TypedSystemRole;

        /**
         * @brief Scans create requests and submits create commands.
         *
         * @param updateCtx Frame-local update context.
         */
        bool update(TUpdateContextType& updateCtx, TCommandBuffer& cmdBuffer) noexcept {

            auto& updateContext = updateCtx.updateContext();

            for (auto [entity, win]: updateContext.template view<
                THandle,
                WindowCreateRequestComponent<THandle>
                >().withActive()) {

                cmdBuffer.template add<WindowCreateCommand<THandle>>(
                    entity.handle(),
                    win->windowConfig
                );
            }
            return true;
        }

    };



}