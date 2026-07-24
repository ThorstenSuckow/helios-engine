/**
 * @file HasFlushParallel.ixx
 * @brief Concept detecting an optional `flushParallel(UpdateContext&)` method on a manager.
 */
module;

#include <concepts>

export module helios.engine.runtime.world.concepts.HasFlushParallel;

import helios.engine.runtime.world.UpdateContext;

export namespace helios::engine::runtime::world::concepts {
    /**
     * @brief Concept detecting an optional `flushParallel(UpdateContext&)` method on a manager.
     *
     * @tparam T The manager type to inspect.
     */
    template<typename T>
    concept HasFlushParallel = requires(T& t, UpdateContext& updateContext) {
        {t.flushParallel(updateContext) } -> std::same_as<void>;
    };
}