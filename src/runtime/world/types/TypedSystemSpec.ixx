/**
 * @file TypedSystemSpec.ixx
 * @brief Compile-time descriptor pairing a system type with its construction arguments.
 */
module;

#include <tuple>

export module helios.engine.runtime.world.types.TypedSystemSpec;

export namespace helios::engine::runtime::world::types {

    /**
     * @brief Stores a system type together with the arguments needed to construct it.
     *
     * Used by the engine's system registration machinery to defer construction
     * until the system is actually installed into a pass.
     *
     * @tparam TSystem The concrete system type.
     * @tparam Args    Constructor argument types captured at the call site.
     *
     * @see TypedSystem
     */
    template<typename TSystem, typename ...Args>
    struct TypedSystemSpec {

        /** @brief The wrapped system type. */
        using System_type = TSystem;

        /** @brief Tuple of decayed constructor arguments forwarded at creation time. */
        std::tuple<std::decay_t<Args>...> args;
    };

    /**
     * @brief Convenience factory that deduces template arguments for `TypedSystemSpec`.
     *
     * ```cpp
     * auto spec = TypedSystem<MySystem>(dep1, dep2);
     * // spec.args holds (dep1, dep2)
     * ```
     *
     * @tparam TSystem The system type to wrap.
     * @tparam Args    Deduced argument types.
     * @param  args    Arguments forwarded into the spec's tuple.
     * @return A `TypedSystemSpec<TSystem, Args...>` holding the provided arguments.
     */
    template<typename TSystem, typename ...Args>
    auto TypedSystem(Args&&...args) {
        return TypedSystemSpec<TSystem, Args...>{
            std::forward_as_tuple(args...)
        };

    }

}