/**
 * @file SystemRole.ixx
 * @brief Tag type for identifying System-role classes at compile time.
 */
module;

export module helios.engine.runtime.world.tags.SystemRole;

export namespace helios::engine::runtime::world::tags {

    /**
     * @brief Compile-time tag identifying a class as a System.
     *
     * @details Declare `using EngineRoleTag = TypedSystemRole;` inside a class
     * to make it satisfy the `IsSystemLike` concept. This enables registration
     * via `SystemRegistry::add<T>()`.
     *
     * @see IsRuntimeSystemLike
     * @see HasTag
     */
    struct TypedSystemRole{};

    /**
     * @brief Compile-time tag identifying a class as a CallableSystem.
     *
     * @details Declare `using EngineRoleTag = CallableSystemRole;` inside a class
     * to make it satisfy the `IsCallableSystemLike` concept. This enables registration
     * via `SystemRegistry::add<T>()`.
     *
     * @see IsRuntimeSystemLike
     * @see HasTag
     */
    struct CallableSystemRole{};
}