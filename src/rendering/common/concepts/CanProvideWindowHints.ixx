/**
 * @file CanProvideWindowHints.ixx
 * @brief Concept requiring a window-hint hook on render backends.
 */
module;

#include <concepts>

export module helios.engine.rendering.common.concepts.CanProvideWindowHints;

export namespace helios::engine::rendering::common::concepts {

    /**
     * @brief Constrains types that provide window/context hints.
     *
     * @tparam T Backend candidate type.
     *
     * @details Requires `void provideWindowHints()`.
     */
    template<typename T>
    concept CanProvideWindowHints = requires(T& t)
    {
        {t.provideWindowHints()}->std::same_as<void>;
    };




}