/**
 * @file WindowCreateSystem.ixx
 * @brief System that translates window-create request components into window-create commands.
 */
module;

export module helios.engine.platform.window.systems.WindowCreateSystem;



import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.concepts;
import helios.ecs.command;



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
     */
    template<typename THandle>
    requires IsWindowHandle<THandle>
    class WindowCreateSystem {

        public:


        using CommandBuffer = ecs::command::TypedCommandBuffer<WindowCreateCommand<THandle>>;

        /**
         * @brief Scans create requests and submits create commands.
         *
         * @param updateContext Frame-local update context.
         */
        void update(UpdateContext& updateContext, CommandBuffer& cmdBuffer) noexcept {

            for (auto [entity, win]: updateContext.template view<
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