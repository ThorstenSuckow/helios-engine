module;

#include <cassert>

export module helios.engine.platform.window.systems.WindowSizeDirtyClearSystem;

import helios.engine.runtime.world.tags.SystemRole;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.platform.window.components;

import helios.ecs.components.Active;
import helios.engine.platform.window.concepts.IsWindowHandle;

import helios.engine.spatial.components.Size2DComponent;

using namespace helios::engine::spatial::components;
using namespace helios::engine::runtime::world::tags;
using namespace helios::engine::platform::window::concepts;
using namespace helios::engine::runtime::world;
using namespace helios::engine::platform::window::components;
using namespace helios::ecs::components;
export namespace helios::engine::platform::window::systems {

    template<typename TMemberHandle>
    requires IsWindowHandle<TMemberHandle>
    class WindowSizeDirtyClearSystem {

    public:

        using EngineRoleTag = TypedSystemRole;

        void update(UpdateContext& updateContext) noexcept {
            for (auto [entity, wc, wsc, active] : updateContext.view<
                TMemberHandle,
                WindowComponent<TMemberHandle>,
                Size2DComponent<TMemberHandle>,
                Active<TMemberHandle>
            >().whereEnabled()) {
                wsc->clearDirty();
            }
        };

    };

}