/**
 * @file renderModes.ixx
 * @brief Marker types that select instanced or non-instanced render paths.
 */
module;

export module helios.engine.rendering.common.types.renderModes;

export namespace helios::engine::rendering::common::types {

    /**
     * @brief Marker for instanced rendering mode.
     */
    struct Instanced{};

    /**
     * @brief Marker for non-instanced rendering mode.
     */
    struct NonInstanced{};

}