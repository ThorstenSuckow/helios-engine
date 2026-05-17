/**
 * @file IsViewportHandle.ixx
 * @brief Concept that constrains a type to the viewport handle type.
 */
module;

#include <concepts>

export module helios.engine.rendering.viewport.concepts.IsViewportHandle;

import helios.engine.rendering.viewport.types.ViewportHandle;

using namespace helios::engine::rendering::viewport::types;
export namespace helios::engine::rendering::viewport::concepts {

    /**
     * @brief Matches `ViewportHandle` exactly.
     *
     * @tparam T Type to test.
     */
    template<typename T>
    concept IsViewportHandle = std::same_as<T, ViewportHandle>;


}