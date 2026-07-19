/**
 * @file WindowCreateSystem.ixx
 * @brief System that translates window-create request components into window-create commands.
 */
module;

export module helios.engine.platform.window.systems.WindowCreateSystem;



import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.messaging.command.NullCommandBuffer;
import helios.engine.runtime.messaging.command.concepts.IsCommandBufferLike;

import helios.engine.runtime.world.tags.SystemRole;

import helios.engine.platform.window.components.WindowCreateRequestComponent;
import helios.engine.platform.window.commands.WindowCreateCommand;

import helios.ecs.components.Active;
import helios.engine.platform.window.concepts.IsWindowHandle;

using namespace helios::engine::platform::window::concepts;
using namespace helios::engine::runtime::world::tags;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::messaging::command;
using namespace helios::engine::runtime::messaging::command::concepts;
using namespace helios::engine::platform::window::components;
using namespace helios::engine::platform::window::commands;
using namespace helios::ecs::components;
using namespace helios::engine::platform::window::components;
export namespace helios::engine::platform::window::systems {

    /**
     * @brief Queues `WindowCreateCommand` for active entities with pending create requests.
     *
     * @tparam THandle Window-domain entity handle type.
     */
    template<typename THandle, typename TCommandBuffer = NullCommandBuffer>
    requires IsWindowHandle<THandle> && IsCommandBufferLike<TCommandBuffer>
    class WindowCreateSystem {

        public:

        using CommandBuffer_type = TCommandBuffer;

        /**
         * @brief Engine role marker used by runtime registries.
         */
        using EngineRoleTag = TypedSystemRole;

        /**
         * @brief Scans create requests and submits create commands.
         *
         * @param updateContext Frame-local update context.
         */
        void update(UpdateContext& updateContext, TCommandBuffer& cmdBuffer) noexcept {

            for (auto [entity, win]: updateContext.view<
                THandle,
                WindowCreateRequestComponent<THandle>
                >().withActive()) {

                cmdBuffer.template add<WindowCreateCommand<THandle>>(
                    entity.handle(),
                    win->windowConfig
                );
            }
        }

    };



}