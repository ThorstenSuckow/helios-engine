/**
 * @file PlatformId.ixx
 * @brief Strong-id type definition for runtime platform entities.
 */
module;

export module helios.platform.environment.types.PlatformId;

import helios.ecs.types.StrongId;
import helios.platform.environment.types.PlatformDomainTag;

using namespace helios::ecs::types;
export namespace helios::platform::environment::types {

    /**
     * @brief Strong id identifying runtime platform entities.
     */
    using PlatformId = StrongId<PlatformDomainTag>;

}