/**
 * @file IsPlatformHandle.ixx
 * @brief Concept constraining runtime platform handle types.
 */
module;

#include <concepts>

export module helios.engine.platform.environment.concepts.IsPlatformHandle;

import helios.engine.platform.environment.types.PlatformHandle;

using namespace helios::engine::platform::environment::types;
export namespace helios::engine::platform::environment::concepts {

    /**
     * @brief True when `T` is exactly `PlatformHandle`.
     */
    template<typename T>
    concept IsPlatformHandle = std::is_same_v<T, PlatformHandle>;

}