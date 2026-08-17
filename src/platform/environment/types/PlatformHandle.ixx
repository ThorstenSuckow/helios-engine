/**
 * @file PlatformHandle.ixx
 * @brief Handle alias for runtime platform entities.
 */
module;

export module helios.engine.platform.environment.types.PlatformHandle;

import helios.ecs.common.types;
import helios.engine.platform.environment.types.PlatformDomainTag;

export namespace helios::engine::platform::environment::types {

    /**
     * @brief ECS handle type referencing runtime platform entities.
     */
    using PlatformHandle = helios::ecs::common::types::EntityHandle<PlatformDomainTag>;

}