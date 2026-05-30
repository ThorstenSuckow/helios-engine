/**
 * @file IsDirtyTrackerLike.ixx
 * @brief Concept that checks whether a type exposes dirty-state tracking operations.
 */
module;

#include <concepts>

export module helios.engine.core.concepts.IsDirtyTrackerLike;

export namespace helios::engine::core::concepts {

    /**
     * @brief Requires `isDirty()` and `clearDirty()` with the expected signatures.
     *
     * @details A type models this concept when it provides
     * `bool isDirty()` and `void clearDirty()`.
     *
     * @tparam T Type to validate.
     */
    template<typename T>
    concept IsDirtyTrackerLike = requires(T& t) {
        { t.isDirty() } -> std::same_as<bool>;
        { t.clearDirty() } -> std::same_as<void>;
    };
}