/**
 * @file WindowSizeUpdateSystem.ixx
 * @brief Updates renderTarget-related state after window size changes.
 */
module;

#include <cassert>

export module helios.engine.platform.window.systems.WindowSizeUpdateSystem;

import helios.ecs.system.tags;
import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.types;
import helios.engine.runtime.concepts;

import helios.engine.platform.window.components;
import helios.ecs.component;
import helios.engine.platform.window.concepts.IsWindowHandle;

import helios.engine.rendering.renderTarget;

import helios.core.log;

import helios.engine.spatial.components.Size2DComponent;

using namespace helios::engine::spatial::components;
using namespace helios::engine::rendering::renderTarget::components;
using namespace helios::engine::rendering::renderTarget::types;

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
     * @tparam TUpdateContextType Type of the UpdateContext to use.
     */
    template<typename TMemberHandle,
             typename TUpdateContextType = types::SystemUpdateContext>
    requires IsWindowHandle<TMemberHandle> &&
             runtime::concepts::ProvidesUpdateContext<TUpdateContextType, UpdateContext>
    class WindowSizeUpdateSystem {

        static inline auto& logger_ = helios::core::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);

    public:

        /** @brief Runtime role tag used for engine system registration. */
        using EcsRoleTag = ecs::system::tags::TypedSystemRole;
        using UpdateContextType = TUpdateContextType;

        /**
         * @brief Processes active windows with dirty size state.
         *
         * @param updateCtx Frame update context.
         */
        bool update(TUpdateContextType& updateCtx) noexcept {
            auto& updateContext = updateCtx.updateContext();
            for (auto [entity, wc, wsc, fbc] : updateContext.template view<
                TMemberHandle,
                WindowComponent<TMemberHandle>,
                Size2DComponent<TMemberHandle>,
                RenderTargetBindingComponent<TMemberHandle>
            >().withActive().template whereAnyDirty<Size2DComponent<TMemberHandle>>()) {

                // c'mon now do something

            }
            return true;
        };

    };

}