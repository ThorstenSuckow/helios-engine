/**
 * @file IsRenderTargetHandle.ixx
 * @brief Concept constraining render target handle types.
 */
module;

#include <concepts>

export module helios.engine.rendering.common.concepts.IsRenderTargetHandle;

import helios.engine.rendering.viewport.types.ViewportHandle;
import helios.engine.rendering.renderTarget.types.RenderTargetHandle;

using namespace helios::engine::rendering::viewport::types;
using namespace helios::engine::rendering::renderTarget::types;
export namespace helios::engine::rendering::common::concepts {

    /**
     * @brief Constrains types to known render target handles.
     *
     * @tparam T Type to validate.
     */
    template<typename T>
    concept IsRenderTargetHandle = std::is_same_v<T, ViewportHandle> ||
                                     std::is_same_v<T, RenderTargetHandle>;

}