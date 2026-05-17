/**
 * @file PlatformId.ixx
 * @brief Strong-id type definition for runtime platform entities.
 */
module;

export module helios.engine.platform.environment.types.PlatformId;

import helios.ecs.types.StrongId;
import helios.engine.platform.environment.types.PlatformDomainTag;

using namespace helios::ecs::types;
export namespace helios::engine::platform::environment::types {

    /**
     * @brief Strong id identifying runtime platform entities.
     */
    using PlatformId = StrongId<PlatformDomainTag>;

}