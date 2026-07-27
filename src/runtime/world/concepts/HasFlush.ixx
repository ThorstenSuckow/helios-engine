/**
 * @file HasFlush.ixx
 * @brief Concept detecting an optional `flush(UpdateContext&)` method on a manager.
 */
module;

#include <concepts>

export module helios.engine.runtime.world.concepts.HasFlush;

import helios.engine.runtime.world.UpdateContext;

export namespace helios::engine::runtime::world::concepts {
    /**
     * @brief Concept detecting an optional `flush(UpdateContext&)` method.
     *
     * @tparam T The type to inspect.
     */
    template<typename T>
    concept HasFlush = requires(T& t, UpdateContext& updateContext) {
        {t.flush(updateContext) } -> std::same_as<void>;
    };
}