/**
 * @file EngineWorld.ixx
 * @brief Aggregate world that routes typed handles to their domain-specific TypedHandleWorld.
 */
module;

#include <imgui_internal.h>
#include <tuple>
#include <type_traits>

export module helios.engine.runtime.world.EngineWorld;

import helios.ecs;

import helios.engine.runtime.world.GameObjectEntityManager;
import helios.engine.runtime.world.ParticleEntityManager;

import helios.engine.platform.window.WindowEntityManager;
import helios.engine.platform.window.concepts;
import helios.engine.platform.window.types;

import helios.engine.platform.environment.PlatformEntityManager;
import helios.engine.platform.environment.types;

import helios.engine.platform.concepts;
import helios.engine.rendering.common.concepts;

import helios.engine.rendering.shader.ShaderEntityManager;
import helios.engine.rendering.material.MaterialEntityManager;
import helios.engine.rendering.mesh.MeshEntityManager;


import helios.engine.rendering.texture.TextureEntityManager;
import helios.engine.rendering.texture.types;

import helios.engine.runtime.world.concepts.IsGameObjectHandle;
import helios.engine.runtime.world.concepts.IsParticleHandle;
import helios.engine.runtime.world.types.GameObjectHandle;

import helios.engine.platform.window.concepts;


import helios.engine.scene.SceneEntityManager;
import helios.engine.scene.CameraEntityManager;
import helios.engine.scene.concepts;

import helios.engine.rendering.viewport.ViewportEntityManager;
import helios.engine.rendering.renderTarget.RenderTargetEntityManager;

import helios.engine.core.TypedTupleCat;

using namespace helios::ecs;
using namespace helios::engine::platform::environment;
using namespace helios::engine::platform::environment::types;
using namespace helios::engine::platform::concepts;
using namespace helios::engine::platform::window;
using namespace helios::engine::platform::window::types;
using namespace helios::engine::platform::window::concepts;
using namespace helios::engine::runtime::world::types;
using namespace helios::engine::runtime::world::concepts;
using namespace helios::engine::rendering::shader;
using namespace helios::engine::rendering::material;
using namespace helios::engine::rendering::mesh;
using namespace helios::engine::rendering::texture;
using namespace helios::engine::rendering::common::concepts;
using namespace helios::engine::rendering::renderTarget;
using namespace helios::engine::rendering::viewport;
using namespace helios::engine::scene;
using namespace helios::engine::scene::concepts;

template<typename T>
inline constexpr bool typed_false = false;


export namespace helios::engine::runtime::world {

    /**
     * @brief Typed world containing game-object entity managers.
     */
    using GameObjectWorld = TypedHandleWorld<GameObjectEntityManager>;

    /**
     * @brief Typed world containing particle entity managers.
     */
    using ParticleWorld = TypedHandleWorld<ParticleEntityManager>;

    /**
     * @brief Typed world containing render-resource entity managers.
     */
    using RenderResourceWorld = TypedHandleWorld<TextureEntityManager, ShaderEntityManager, MaterialEntityManager, MeshEntityManager>;

    /**
     * @brief Typed world containing platform-related entity managers.
     */
    using PlatformWorld = TypedHandleWorld<WindowEntityManager, PlatformEntityManager>;

    /**
     * @brief Typed world containing render-view related entity managers.
     */
    using RenderViewWorld = TypedHandleWorld<RenderTargetEntityManager, SceneEntityManager, CameraEntityManager, ViewportEntityManager>;

    /**
     * @brief Concatenated tuple of all entity-manager types used by `EngineWorld`.
     */
    using RegisteredEntityManagers = helios::engine::core::TypedTupleCat<
        GameObjectWorld::EntityManager_types,
        RenderResourceWorld::EntityManager_types,
        PlatformWorld::EntityManager_types,
        RenderViewWorld::EntityManager_types,
        ParticleWorld::EntityManager_types
    >::type;

    /** @brief Minimal type-list used to carry handle types without instantiation. */
    template<typename... Ts> struct type_list {};

    /**
     * @brief Extracts the `Handle_type` of every entity manager in a manager tuple.
     *
     * @tparam TManagerTuple `std::tuple` of entity manager types.
     */
    template<typename TManagerTuple>
    struct HandlesOf;
    template<typename... TManagers>
    struct HandlesOf<std::tuple<TManagers...>> {
        using type = type_list<typename TManagers::Handle_type...>;
    };
    /**
     * @brief Convenience alias that resolves to the `type_list` of all handle types for a `TypedHandleWorld`.
     *
     * @tparam TWorld A `TypedHandleWorld` specialisation.
     */
    template<typename TWorld>
    using WorldHandles = typename HandlesOf<typename TWorld::EntityManager_types>::type;


    /**
     * @brief Top-level aggregate world that dispatches entity operations to the
     *        correct domain-specific `TypedHandleWorld` based on the handle type.
     *
     * Routes `add`, `find`, `view`, `destroy`, `clone`, `clearDirtySets`, etc.
     * to one of the sub-worlds.
     */
    class EngineWorld {

        GameObjectWorld gameObjectWorld_{};
        RenderResourceWorld renderResourceWorld_{};
        PlatformWorld platformWorld_{};
        RenderViewWorld renderViewWorld_{};
        ParticleWorld particleWorld_{};

        /**
         * @brief Clears all dirty sets for every handle type in `THandles` inside `world`.
         *
         * @tparam TWorld      Sub-world type.
         * @tparam TComponents Component types to clear (empty = clear all).
         * @tparam THandles    Handle types derived from the world's entity managers.
         */
        template<typename TWorld, typename... TComponents, typename... THandles>
        void clearDirtySetsForHandles(TWorld& world, type_list<THandles...>) {
            (world.template clearDirtySets<THandles, TComponents...>(), ...);
        }

    public:

        /**
         * @brief Returns the game-object and scene sub-world.
         */
        [[nodiscard]] GameObjectWorld& gameObjectWorld() {
            return gameObjectWorld_;
        }

        /**
         * @brief Returns the platform (window, environment) sub-world.
         */
        [[nodiscard]] PlatformWorld& platformWorld() {
            return platformWorld_;
        }

        /**
         * @brief Returns the render-resource (shader, material, mesh) sub-world.
         */
        [[nodiscard]] RenderResourceWorld& renderResourceWorld() {
            return renderResourceWorld_;
        }

        /**
         * @brief Returns the render-target and viewport sub-world.
         */
        [[nodiscard]] RenderViewWorld& renderViewWorld() {
            return renderViewWorld_;
        }

        /**
         * @brief Returns the particle sub-world.
         */
        [[nodiscard]] ParticleWorld& particleWorld() {
            return particleWorld_;
        }

        /**
         * @brief Copies an entity within the sub-world appropriate for `THandle`.
         *
         * @tparam THandle Handle type of the entity to copy.
         * @param  source  Handle of the source entity.
         * @return `Entity` wrapper for the newly copied entity.
         */
        template<typename THandle>
        [[nodiscard]] auto copyEntity(THandle source) noexcept {

            if constexpr(IsGameObjectHandle<THandle>) {
                return gameObjectWorld_.copyEntity<THandle>(source);
            } else if constexpr(IsAnyPlatformHandle<THandle>){
                return platformWorld_.copyEntity<THandle>(source);
            } else if constexpr(IsRenderResourceHandle<THandle>) {
                return renderResourceWorld_.copyEntity<THandle>(source);
            } else if constexpr(IsRenderViewHandle<THandle>) {
                return renderViewWorld_.copyEntity<THandle>(source);
            } else if constexpr(IsParticleHandle<THandle>) {
                return particleWorld_.copyEntity<THandle>(source);
            } else {
                static_assert(typed_false<THandle>, "Unsupported handle type for copying");
            }
        }

        /**
         * @brief Returns the entity manager responsible for `THandle`.
         *
         * @tparam THandle Handle type whose manager is requested.
         * @return Reference to the matching entity manager.
         */
        template<typename THandle>
        [[nodiscard]] auto& entityManager() noexcept {

            if constexpr(IsGameObjectHandle<THandle>) {
                return gameObjectWorld_.entityManager<THandle>();
            } else if constexpr(IsAnyPlatformHandle<THandle>){
                return platformWorld_.entityManager<THandle>();
            } else if constexpr(IsRenderResourceHandle<THandle>) {
                return renderResourceWorld_.entityManager<THandle>();
            } else if constexpr(IsRenderViewHandle<THandle>) {
                return renderViewWorld_.entityManager<THandle>();
            } else if constexpr(IsParticleHandle<THandle>) {
                return particleWorld_.entityManager<THandle>();
            } else {
                static_assert(typed_false<THandle>, "Unsupported handle type for entityManager");
            }
        }

        /**
         * @brief Returns the `SparseSet<TComponent>` from the entity manager for `THandle`.
         *
         * @tparam THandle    Handle type identifying the sub-world.
         * @tparam TComponent Component type whose storage is requested.
         * @return Non-owning pointer to the sparse set, or `nullptr` if not allocated.
         */
        template<typename THandle, typename TComponent>
        [[nodiscard]] auto* sparseSet() noexcept {

            if constexpr(IsGameObjectHandle<THandle>) {
                return gameObjectWorld_.entityManager<THandle>().template sparseSet<TComponent>();
            } else if constexpr(IsAnyPlatformHandle<THandle>){
                return platformWorld_.entityManager<THandle>().template sparseSet<TComponent>();
            } else if constexpr(IsRenderResourceHandle<THandle>) {
                return renderResourceWorld_.entityManager<THandle>().template sparseSet<TComponent>();
            } else if constexpr(IsRenderViewHandle<THandle>) {
                return renderViewWorld_.entityManager<THandle>().template sparseSet<TComponent>();
            } else if constexpr(IsParticleHandle<THandle>) {
                return particleWorld_.entityManager<THandle>().template sparseSet<TComponent>();
            } else {
                static_assert(typed_false<THandle>, "Unsupported handle type for storage");
            }
        }

        /**
         * @brief Returns whether `handle` refers to a living entity.
         *
         * @tparam THandle Deduced handle type.
         * @param  handle  Handle to validate.
         */
        template<typename THandle>
        [[nodiscard]] bool isValid(THandle handle) noexcept {

            if constexpr(IsGameObjectHandle<THandle>) {
                return gameObjectWorld_.entityManager<THandle>().isValid(handle);
            } else if constexpr(IsAnyPlatformHandle<THandle>){
                return platformWorld_.entityManager<THandle>().isValid(handle);
            } else if constexpr(IsRenderResourceHandle<THandle>) {
                return renderResourceWorld_.entityManager<THandle>().isValid(handle);
            } else if constexpr(IsRenderViewHandle<THandle>) {
                return renderViewWorld_.entityManager<THandle>().isValid(handle);
            } else if constexpr(IsParticleHandle<THandle>) {
                return particleWorld_.entityManager<THandle>().isValid(handle);
            } else {
                static_assert(typed_false<THandle>, "Unsupported handle type for storage");
            }
        }

        /**
         * @brief Finds and returns an `Entity` wrapper for `handle`.
         *
         * @tparam THandle Deduced handle type.
         * @param  handle  Handle to look up.
         * @return `Entity` wrapper, or an invalid wrapper if not found.
         */
        template<typename THandle>
        [[nodiscard]] auto find(THandle handle) noexcept {

            if constexpr(IsGameObjectHandle<THandle>) {
                return gameObjectWorld_.findEntity<THandle>(handle);
            } else if constexpr(IsAnyPlatformHandle<THandle>) {
                return platformWorld_.findEntity<THandle>(handle);
            } else  if constexpr(IsRenderResourceHandle<THandle>) {
                return renderResourceWorld_.findEntity<THandle>(handle);
            } else if constexpr(IsRenderViewHandle<THandle>) {
                return renderViewWorld_.findEntity<THandle>(handle);
            } else if constexpr(IsParticleHandle<THandle>) {
                return particleWorld_.findEntity<THandle>(handle);
            } else {
                static_assert(typed_false<THandle>, "Unsupported handle type for searching");
            }
        }

        /**
         * @brief Creates a new entity in the sub-world appropriate for `THandle`.
         *
         * @tparam THandle   Handle type that determines the target sub-world.
         * @param  strongId  Optional strong identifier for the new entity.
         * @return `Entity` wrapper for the newly created entity.
         */
        template<typename THandle>
        [[nodiscard]] auto add(typename THandle::StrongId_type strongId = typename THandle::StrongId_type{}) {
            if constexpr(IsGameObjectHandle<THandle>) {
                return gameObjectWorld_.addEntity<THandle>(strongId);
            } else if constexpr(IsAnyPlatformHandle<THandle>) {
                return platformWorld_.addEntity<THandle>(strongId);
            } else if constexpr(IsRenderResourceHandle<THandle>) {
                return renderResourceWorld_.addEntity<THandle>(strongId);
            } else if constexpr(IsRenderViewHandle<THandle>) {
                return renderViewWorld_.addEntity<THandle>(strongId);
            } else if constexpr(IsParticleHandle<THandle>) {
                return particleWorld_.addEntity<THandle>(strongId);
            } else {
                static_assert(typed_false<THandle>, "Unsupported handle type for adding");
            }
        }

        /**
         * @brief Returns a `View` over entities with `TComponents` in the sub-world for `THandle`.
         *
         * @tparam THandle     Handle type identifying the sub-world.
         * @tparam TComponents Required component types.
         */
        template<typename THandle, typename... TComponents>
        [[nodiscard]] auto view() {
            if constexpr(IsGameObjectHandle<THandle>) {
                return gameObjectWorld_.template view<THandle, TComponents...>();
            } else if constexpr(IsAnyPlatformHandle<THandle>) {
                return platformWorld_.template view<THandle, TComponents...>();
            } else if constexpr(IsRenderResourceHandle<THandle>) {
                return renderResourceWorld_.template view<THandle, TComponents...>();
            } else if constexpr(IsRenderViewHandle<THandle>) {
                return renderViewWorld_.template view<THandle, TComponents...>();
            } else if constexpr(IsParticleHandle<THandle>) {
                return particleWorld_.template view<THandle, TComponents...>();
            } else {
                static_assert(typed_false<THandle>, "Unsupported handle type for viewing");
            }

        }

        /**
         * @brief Destroys an entity in the sub-world appropriate for `THandle`.
         *
         * @tparam THandle Deduced handle type.
         * @param  handle  Handle of the entity to destroy.
         * @return `true` if destroyed, `false` if the handle was already invalid.
         */
        template<typename THandle>
        [[nodiscard]] auto destroy(const THandle handle) {
            if constexpr(IsGameObjectHandle<THandle>) {
                return gameObjectWorld_.destroy<THandle>(handle);
            } else if constexpr(IsAnyPlatformHandle<THandle>) {
                return platformWorld_.destroy<THandle>(handle);
            } else  if constexpr(IsRenderResourceHandle<THandle>) {
                return renderResourceWorld_.destroy<THandle>(handle);
            } else if constexpr(IsRenderViewHandle<THandle>) {
                return renderViewWorld_.destroy<THandle>(handle);
            } else if constexpr(IsParticleHandle<THandle>) {
                return particleWorld_.destroy<THandle>(handle);
            } else {
                static_assert(typed_false<THandle>, "Unsupported handle type for destroying");
            }
        }

        /**
         * @brief Clears dirty sets across all sub-worlds or for a specific handle and component list.
         *
         * When called as `clearDirtySets<>()` (no template args), clears all dirty sets in every
         * sub-world. When called as `clearDirtySets<THandle, TComponents...>()`, only the matching
         * sub-world and component types are affected.
         *
         * @tparam THandle     Handle type selecting the sub-world (`void` = all worlds).
         * @tparam TComponents Component types whose dirty sets are cleared (empty = all).
         */
        template<typename THandle = void, typename... TComponents>
        void clearDirtySets() {

            if constexpr (std::is_same_v<THandle, void>) {

                clearDirtySetsForHandles<GameObjectWorld>(gameObjectWorld_, WorldHandles<GameObjectWorld>{});
                clearDirtySetsForHandles<PlatformWorld>(platformWorld_, WorldHandles<PlatformWorld>{});
                clearDirtySetsForHandles<RenderResourceWorld>(renderResourceWorld_, WorldHandles<RenderResourceWorld>{});
                clearDirtySetsForHandles<RenderViewWorld>(renderViewWorld_, WorldHandles<RenderViewWorld>{});
                clearDirtySetsForHandles<ParticleWorld>(particleWorld_, WorldHandles<ParticleWorld>{});

            } else if constexpr(IsGameObjectHandle<THandle>) {
                gameObjectWorld_.template clearDirtySets<THandle, TComponents...>();
            } else if constexpr(IsAnyPlatformHandle<THandle>) {
                platformWorld_.template clearDirtySets<THandle, TComponents...>();
            } else if constexpr(IsRenderResourceHandle<THandle>) {
                renderResourceWorld_.template clearDirtySets<THandle, TComponents...>();
            } else if constexpr(IsRenderViewHandle<THandle>) {
                renderViewWorld_.template clearDirtySets<THandle, TComponents...>();
            } else if constexpr(IsParticleHandle<THandle>) {
                particleWorld_.template clearDirtySets<THandle, TComponents...>();
            } else {
                static_assert(typed_false<THandle>, "Unsupported handle type for clearDirtySets()");
            }

        }



    };

}