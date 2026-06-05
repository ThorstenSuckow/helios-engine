/**
 * @file ComponentTypeTags.ixx
 * @brief Marker tag types for component classification in helios engine core types.
 */
module;

export module helios.engine.core.types.ComponentTypeTags;

export namespace helios::engine::core::types {

    /**
     * @brief Marks a component as local to an entity or scope.
     */
    struct Local {};

    /**
     * @brief Marks a component as world-level or globally shared.
     */
    struct World {};

    /**
     * @brief Marks a component as state-bearing runtime data.
     */
    struct State {};

    /**
     * @brief Marks a component as canonical/authoritative data.
     */
    struct Canonical {};

}