/**
 * @file IsSystemLike.ixx
 * @brief Concept constraining types eligible for system registration.
 */
module;

#include <concepts>

export module helios.engine.runtime.world.concepts.IsSystemLike;

import helios.engine.runtime.concepts.HasTag;
import helios.engine.runtime.world.tags.SystemRole;

import helios.engine.runtime.world.UpdateContext;

using namespace helios::engine::runtime::concepts;
export namespace helios::engine::runtime::world::concepts {


    /**
     * @brief Concept that constrains a type to a TypedSystem-like type.
     *
     * @tparam T The type to inspect.
     */
    template<class T>
    concept IsTypedSystemLike = (requires(T& t, helios::engine::runtime::world::UpdateContext& updateContext) {
        {t.update(updateContext) } -> std::same_as<void>;
    } || requires(T& t, helios::engine::runtime::world::UpdateContext& updateContext, typename T::CommandBuffer_type& buf)
    {
        t.update(updateContext, buf);
    }) && HasTag<T, tags::TypedSystemRole>;


    /**
     * @brief Concept that constrains a type to a callable-backed system.
     *
     * @tparam T The type to inspect.
     */
    template<class T>
     concept IsCallableSystemLike = (requires(T& t, helios::engine::runtime::world::UpdateContext& updateContext) {
         {t.update(updateContext) } -> std::same_as<void>;
     } || requires(T& t, helios::engine::runtime::world::UpdateContext& updateContext, typename T::CommandBuffer_type& buf)
     {
         t.update(updateContext, buf);
     }) && HasTag<T, tags::CallableSystemRole>;

    template<class T>
    concept IsRuntimeSystemLike = IsTypedSystemLike<T> || IsCallableSystemLike<T>;


}
