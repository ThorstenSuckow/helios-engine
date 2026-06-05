/**
 * @file ComponentTypeSpecs.ixx
 * @brief Component template specifications for the helios engine core types.
 */
module;

export module helios.engine.core.types.ComponentTypeSpecs;

export namespace helios::engine::core::types {

    /**
     * @brief Binds trailing template arguments to a component template.
     * @tparam TComponent Component template in the form `TComponent<THandle, ...>`.
     * @tparam TArgs Fixed template arguments appended after `THandle`.
     */
    template<template<typename, typename...> typename TComponent, typename... TArgs>
    struct ComponentTypeSpec {
        /**
         * @brief Resolves the concrete component type for a handle type.
         * @tparam THandle Handle type used as the first template argument.
         */
        template<typename THandle>
        using type = TComponent<THandle, TArgs...>;
    };

    /**
     * @brief Semantic alias for mutable-component specifications.
     */
    template<template<typename, typename...> typename TComponent, typename... TArgs>
    using DirtyComponentSpec = ComponentTypeSpec<TComponent, TArgs...>;

}