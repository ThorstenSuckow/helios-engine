/**
 * @file IsAnyPlatformHandle.ixx
 * @brief Concept accepting all platform-domain handle types.
 */
module;

#include <concepts>

export module helios.engine.platform.concepts.IsAnyPlatformHandle;

import helios.engine.platform.environment.concepts;
import helios.engine.platform.window.concepts;

using namespace helios::engine::platform::environment::concepts;
using namespace helios::engine::platform::window::concepts;
export namespace helios::engine::platform::concepts {

    /**
     * @brief True when `THandle` is either a runtime platform handle or a window handle.
     */
    template<typename THandle>
    concept IsAnyPlatformHandle = IsPlatformHandle<THandle> || IsWindowHandle<THandle>;

}
