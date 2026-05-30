/**
 * @file RenderTargetSize.ixx
 * @brief Value type describing a renderTarget size in pixels.
 */
module;

export module helios.engine.rendering.renderTarget.types.RenderTargetSize;

import helios.math;

using namespace helios::math;
export namespace helios::engine::rendering::renderTarget::types {

    /**
     * @brief Width/height pair used for renderTarget resize operations.
     */
    using RenderTargetSize = helios::math::vec2f;

} // namespace helios::engine::rendering::platform::types
