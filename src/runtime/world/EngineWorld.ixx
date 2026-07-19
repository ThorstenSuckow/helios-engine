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

import helios.engine.runtime.world.concepts.IsGameObjectHandle;
import helios.engine.runtime.world.types.GameObjectHandle;

import helios.engine.platform.window.concepts;
import helios.engine.rendering.common.concepts.IsRenderResourceHandle;

import helios.engine.rendering.common.concepts;

import helios.engine.scene.SceneEntityManager;
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
using namespace helios::engine::rendering::common::concepts;
using namespace helios::engine::rendering::renderTarget;
using namespace helios::engine::rendering::viewport;
using namespace helios::engine::scene;
using namespace helios::engine::scene::concepts;

template<typename T>
inline constexpr bool typed_false = false;

template<typename T>
concept IsGameplaySystemHandle = IsGameObjectHandle<T> || helios::engine::scene::concepts::IsSceneHandle<T>;

export namespace helios::engine::runtime::world {

    /**
     * @brief Typed world containing game-object entity managers.
     */
    using GameObjectWorld = TypedHandleWorld<GameObjectEntityManager, SceneEntityManager>;

    /**
     * @brief Typed world containing render-resource entity managers.
     */
    using RenderResourceWorld = TypedHandleWorld<ShaderEntityManager, MaterialEntityManager, MeshEntityManager>;

    /**
     * @brief Typed world containing platform-related entity managers.
     */
    using PlatformWorld = TypedHandleWorld<WindowEntityManager, PlatformEntityManager>;

    /**
     * @brief Typed world containing render-target entity managers.
     */
    using RenderTargetWorld = TypedHandleWorld<RenderTargetEntityManager, ViewportEntityManager>;

    /**
     * @brief Concatenated tuple of all entity-manager types used by `EngineWorld`.
     */
    using RegisteredEntityManagers = helios::engine::core::TypedTupleCat<
        GameObjectWorld::EntityManager_types,
        RenderResourceWorld::EntityManager_types,
        PlatformWorld::EntityManager_types,
        RenderTargetWorld::EntityManager_types
    >::type;

    template<typename... Ts> struct type_list {};

    template<typename TManagerTuple>
    struct HandlesOf;
    template<typename... TManagers>
    struct HandlesOf<std::tuple<TManagers...>> {
        using type = type_list<typename TManagers::Handle_type...>;
    };
    template<typename TWorld>
    using WorldHandles = typename HandlesOf<typename TWorld::EntityManager_types>::type;


    /**
     * @brief Aggregate runtime world for game objects, platform entities, and rendering domains.
     */
    class EngineWorld {

        GameObjectWorld gameObjectWorld_{};
        RenderResourceWorld renderResourceWorld_{};
        PlatformWorld platformWorld_{};
        RenderTargetWorld renderTargetWorld_{};

        template<typename TWorld, typename... TComponents, typename... THandles>
        void clearDirtySetsForHandles(TWorld& world, type_list<THandles...>) {
            (world.template clearDirtySets<THandles, TComponents...>(), ...);
        }

    public:

        [[nodiscard]] GameObjectWorld& gameObjectWorld() {
            return gameObjectWorld_;
        }

        [[nodiscard]] PlatformWorld& platformWorld() {
            return platformWorld_;
        }

        [[nodiscard]] RenderResourceWorld& renderResourceWorld() {
            return renderResourceWorld_;
        }

        [[nodiscard]] RenderTargetWorld& renderTargetWorld() {
            return renderTargetWorld_;
        }

        template<typename THandle>
        [[nodiscard]] auto clone(THandle source) noexcept {

            if constexpr(IsGameplaySystemHandle<THandle>) {
                return gameObjectWorld_.cloneEntity<THandle>(source);
            } else if constexpr(IsAnyPlatformHandle<THandle>){
                return platformWorld_.cloneEntity<THandle>(source);
            } else if constexpr(IsRenderResourceHandle<THandle>) {
                return renderResourceWorld_.cloneEntity<THandle>(source);
            } else if constexpr(IsRenderTargetHandle<THandle>) {
                return renderTargetWorld_.cloneEntity<THandle>(source);
            } else {
                static_assert(typed_false<THandle>, "Unsupported handle type for cloning");
            }
        }


        template<typename THandle>
        [[nodiscard]] auto find(THandle handle) noexcept {
            if constexpr(IsGameplaySystemHandle<THandle>) {
                return gameObjectWorld_.findEntity<THandle>(handle);
            } else if constexpr(IsAnyPlatformHandle<THandle>) {
                return platformWorld_.findEntity<THandle>(handle);
            } else  if constexpr(IsRenderResourceHandle<THandle>) {
                return renderResourceWorld_.findEntity<THandle>(handle);
            } else if constexpr(IsRenderTargetHandle<THandle>) {
                return renderTargetWorld_.findEntity<THandle>(handle);
            } else {
                static_assert(typed_false<THandle>, "Unsupported handle type for searching");
            }
        }

        template<typename THandle>
        [[nodiscard]] auto add(typename THandle::StrongId_type strongId = typename THandle::StrongId_type{}) {
            if constexpr(IsGameplaySystemHandle<THandle>) {
                return gameObjectWorld_.addEntity<THandle>(strongId);
            } else if constexpr(IsAnyPlatformHandle<THandle>) {
                return platformWorld_.addEntity<THandle>(strongId);
            } else if constexpr(IsRenderResourceHandle<THandle>) {
                return renderResourceWorld_.addEntity<THandle>(strongId);
            } else if constexpr(IsRenderTargetHandle<THandle>) {
                return renderTargetWorld_.addEntity<THandle>(strongId);
            }  else {
                static_assert(typed_false<THandle>, "Unsupported handle type for adding");
            }
        }


         template<typename THandle, typename... TComponents>
         [[nodiscard]] auto view() {
            if constexpr(IsGameplaySystemHandle<THandle>) {
                return gameObjectWorld_.template view<THandle, TComponents...>();
            } else if constexpr(IsAnyPlatformHandle<THandle>) {
                return platformWorld_.template view<THandle, TComponents...>();
            } else if constexpr(IsRenderResourceHandle<THandle>) {
                return renderResourceWorld_.template view<THandle, TComponents...>();
            } else if constexpr(IsRenderTargetHandle<THandle>) {
                return renderTargetWorld_.template view<THandle, TComponents...>();
            } else {
                static_assert(typed_false<THandle>, "Unsupported handle type for viewing");
            }

        }

         template<typename THandle>
         [[nodiscard]] auto destroy(const THandle handle) {
            if constexpr(IsGameplaySystemHandle<THandle>) {
                return gameObjectWorld_.destroy<THandle>(handle);
            } else if constexpr(IsAnyPlatformHandle<THandle>) {
                return platformWorld_.destroy<THandle>(handle);
            } else  if constexpr(IsRenderResourceHandle<THandle>) {
                return renderResourceWorld_.destroy<THandle>(handle);
            } else if constexpr(IsRenderTargetHandle<THandle>) {
                return renderTargetWorld_.destroy<THandle>(handle);
            } else {
                static_assert(typed_false<THandle>, "Unsupported handle type for destroying");
            }
        }

        template<typename THandle = void, typename... TComponents>
        void clearDirtySets() {

            if constexpr (std::is_same_v<THandle, void>) {

                clearDirtySetsForHandles<GameObjectWorld>(gameObjectWorld_, WorldHandles<GameObjectWorld>{});
                clearDirtySetsForHandles<PlatformWorld>(platformWorld_, WorldHandles<PlatformWorld>{});
                clearDirtySetsForHandles<RenderResourceWorld>(renderResourceWorld_, WorldHandles<RenderResourceWorld>{});
                clearDirtySetsForHandles<RenderTargetWorld>(renderTargetWorld_, WorldHandles<RenderTargetWorld>{});

            } else if constexpr(IsGameplaySystemHandle<THandle>) {
                gameObjectWorld_.template clearDirtySets<THandle, TComponents...>();
            } else if constexpr(IsAnyPlatformHandle<THandle>) {
                platformWorld_.template clearDirtySets<THandle, TComponents...>();
            } else if constexpr(IsRenderResourceHandle<THandle>) {
                renderResourceWorld_.template clearDirtySets<THandle, TComponents...>();
            } else if constexpr(IsRenderTargetHandle<THandle>) {
                renderTargetWorld_.template clearDirtySets<THandle, TComponents...>();
            } else {
                static_assert(typed_false<THandle>, "Unsupported handle type for clearDirtySets()");
            }

        }



    };

}