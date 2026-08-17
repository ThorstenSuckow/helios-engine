/**
 * @file HasTag.ixx
 * @brief Concept for detecting a compile-time engine role tag on a type.
 */
module;

#include <concepts>

export module helios.engine.runtime.concepts.HasTag;

export namespace helios::engine::runtime::concepts {

    /**
     * @brief Detects whether T declares a nested `EcsRoleTag` alias equal to Tag.
     *
     * @details Used by role-specific concepts (IsManagerLike, IsSystemLike) to verify
     * that a type has been explicitly tagged for a particular engine role.
     * The tag must be declared as `using EcsRoleTag = Tag;` inside the class.
     *
     * @tparam T The type to inspect.
     * @tparam Tag The expected tag type (e.g. ManagerRole, SystemRole).
     *
     * @see ManagerRole
     * @see SystemRole
     */
    template<class T, class Tag>
    concept HasTag = requires { typename T::EcsRoleTag; } &&
                 std::same_as<typename T::EcsRoleTag, Tag>;

}