/**
 * @file WindowSizeUpdateSystem.ixx
 * @brief Updates renderTarget-related state after window size changes.
 */
module;

#include <cassert>

export module helios.engine.platform.window.systems.WindowSizeUpdateSystem;

import helios.engine.runtime.world.tags.SystemRole;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.platform.window.components;
import helios.ecs.components.Active;
import helios.engine.platform.window.concepts.IsWindowHandle;

import helios.engine.rendering.renderTarget;

import helios.engine.util.log;

import helios.engine.spatial.components.Size2DComponent;

using namespace helios::engine::spatial::components;
using namespace helios::engine::rendering::renderTarget::components;
using namespace helios::engine::rendering::renderTarget::types;
using namespace helios::engine::runtime::world::tags;
using namespace helios::engine::platform::window::concepts;
using namespace helios::engine::runtime::world;
using namespace helios::engine::platform::window::components;
using namespace helios::ecs::components;

#define HELIOS_LOG_SCOPE "helios::engine::platform::window::systems::WindowSizeUpdateSystem"
export namespace helios::engine::platform::window::systems {

    /**
     * @brief System that reacts to dirty window size components.
     *
     * @tparam TMemberHandle Window entity handle type.
     */
    template<typename TMemberHandle>
    requires IsWindowHandle<TMemberHandle>
    class WindowSizeUpdateSystem {

        static inline auto& logger_ = helios::engine::util::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);

    public:

        /** @brief Runtime role tag used for engine system registration. */
        using EngineRoleTag = TypedSystemRole;

        /**
         * @brief Processes active windows with dirty size state.
         *
         * @param updateContext Frame update context.
         */
        void update(UpdateContext& updateContext) noexcept {
            for (auto [entity, wc, wsc, fbc, active] : updateContext.view<
                TMemberHandle,
                WindowComponent<TMemberHandle>,
                Size2DComponent<TMemberHandle>,
                RenderTargetBindingComponent<TMemberHandle>,
                Active<TMemberHandle>
            >().whereEnabled()) {
                if (wsc->isDirty()) {
                    // c'mon now do something
                }
            }
        };

    };

}