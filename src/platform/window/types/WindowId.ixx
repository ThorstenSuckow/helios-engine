/**
 * @file WindowId.ixx
 * @brief Strong-id type definition for window domain entities.
 */
module;

export module helios.engine.platform.window.types.WindowId;

import helios.ecs.types.StrongId;
import helios.engine.platform.window.types.WindowDomainTag;

using namespace helios::ecs::types;
export namespace helios::engine::platform::window::types {

    /**
     * @brief Strong id identifying window entities.
     */
    using WindowId = StrongId<WindowDomainTag>;

}