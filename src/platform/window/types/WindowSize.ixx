/**
 * @file WindowSize.ixx
 * @brief Value type describing a window size in pixels.
 */
module;

export module helios.engine.platform.window.types.WindowSize;

import helios.math;

using namespace helios::math;
export namespace helios::engine::platform::window::types {

    /**
     * @brief Width/height pair used for window creation and resize operations.
     */
    using WindowSize = helios::math::vec2f;

} // namespace helios::engine::platform::window::types
