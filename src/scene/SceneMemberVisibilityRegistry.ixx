/**
 * @file SceneMemberVisibilityRegistry.ixx
 * @brief Registry for visible and culled scene members grouped by viewport and submission mode.
 */
module;

#include "helios-engine-config.h"
#include <span>
#include <cstdint>
#include <vector>

export module helios.engine.scene.SceneMemberVisibilityRegistry;

import helios.engine.core.container.HandleManyToManyMap;

import helios.engine.scene.types;

import helios.engine.rendering.common.types;
import helios.engine.rendering.viewport.types;
import helios.engine.rendering.renderTarget.types;

using namespace helios::engine::rendering::viewport::types;
using namespace helios::engine::rendering::common::types;
using namespace helios::engine::scene::types;
using namespace helios::engine::rendering::common::types;
using namespace helios::engine::rendering::renderTarget::types;
using namespace helios::engine::core::container;

namespace {
    template<class>
    inline constexpr bool always_false_v = false;
}

export namespace helios::engine::scene {


    /**
     * @brief Stores per-viewport visibility/culling results for scene members.
     *
     * The registry separates members by submission mode (`Instanced` vs `NonInstanced`)
     * and by viewport handle index for fast retrieval during render submission.
     *
     * @tparam TMemberHandle Handle type used to reference scene members.
     */
    template<typename TMemberHandle>
    class SceneMemberVisibilityRegistry {


        struct RenderTargetViewportSceneKey {
            std::uint32_t a;
            std::uint32_t b;
            std::uint32_t c;

            friend bool operator==(const RenderTargetViewportSceneKey&, const RenderTargetViewportSceneKey&) = default;
        };

        std::vector<RenderTargetViewportSceneKey> renderTargetViewportSceneKeys_;

        std::vector<std::vector<SceneMemberVisibilityContext<TMemberHandle, Instanced>>> visibilityContextsInstanced_;
        std::vector<std::vector<SceneMemberVisibilityContext<TMemberHandle, Instanced>>> culledContextsInstanced_;

        std::vector<std::vector<SceneMemberVisibilityContext<TMemberHandle, NonInstanced>>> visibilityContextsNonInstanced_;
        std::vector<std::vector<SceneMemberVisibilityContext<TMemberHandle, NonInstanced>>> culledContextsNonInstanced_;

        std::vector<SceneRenderContext<TMemberHandle>> sceneRenderContexts_;


    public:

        /**
         * @brief Constructs the registry with pre-reserved top-level storage.
         */
        SceneMemberVisibilityRegistry() {
            visibilityContextsInstanced_.reserve(DEFAULT_VIEWPORT_POOL_CAPACITY);
            for (auto& vec : visibilityContextsInstanced_) {
                vec.reserve(HELIOS_DEFAULT_VISIBLE_MEMBER_CAPACITY);
            }
            culledContextsInstanced_.reserve(DEFAULT_VIEWPORT_POOL_CAPACITY);
            for (auto& vec : culledContextsInstanced_) {
                vec.reserve(HELIOS_DEFAULT_VISIBLE_MEMBER_CAPACITY);
            }
            visibilityContextsNonInstanced_.reserve(DEFAULT_VIEWPORT_POOL_CAPACITY);
            for (auto& vec : visibilityContextsNonInstanced_) {
                vec.reserve(HELIOS_DEFAULT_VISIBLE_MEMBER_CAPACITY);
            }
            culledContextsNonInstanced_.reserve(DEFAULT_VIEWPORT_POOL_CAPACITY);
            for (auto& vec : culledContextsNonInstanced_) {
                vec.reserve(HELIOS_DEFAULT_VISIBLE_MEMBER_CAPACITY);
            }

            sceneRenderContexts_.reserve(DEFAULT_VIEWPORT_POOL_CAPACITY);
        }


        /**
         * @brief Adds a scene render context if it is not already present.
         * @param context Render context to insert.
         */
        void addSceneRenderContext(SceneRenderContext<TMemberHandle>&& context) {

            for (const auto& existingContext : sceneRenderContexts_) {
                if (existingContext == context) {
                    return;
                }
            }

            sceneRenderContexts_.push_back(std::move(context));
        }

        /**
         * @brief Returns all registered scene render contexts.
         */
        [[nodiscard]] std::span<const SceneRenderContext<TMemberHandle>> sceneRenderContexts() const noexcept {
            return sceneRenderContexts_;
        }



        /**
         * @brief Adds a culled member visibility context for a viewport.
         * @tparam TSubmissionMode Submission mode (`Instanced` or `NonInstanced`).
         * @param viewportHandle Target viewport.
         * @param context Visibility context to append.
         */
        template<typename TSubmissionMode>
        void addCulledMember(const ViewportHandle viewportHandle, SceneMemberVisibilityContext<TMemberHandle, TSubmissionMode>&& context) {
            const auto idx = viewportHandle.entityId;
            if constexpr (std::is_same_v<TSubmissionMode, NonInstanced>) {
                if (culledContextsNonInstanced_.size() <= idx) {
                    culledContextsNonInstanced_.resize(idx + 1);
                }
                culledContextsNonInstanced_[idx].emplace_back(std::move(context));
            } else if constexpr (std::is_same_v<TSubmissionMode, Instanced>) {
                if (culledContextsInstanced_.size() <= idx) {
                    culledContextsInstanced_.resize(idx + 1);
                }
                culledContextsInstanced_[idx].emplace_back(std::move(context));
            } else {
                static_assert(false, "Unsupported submission mode");
            }
        }


        /**
         * @brief Adds a visible member visibility context for a viewport.
         * @tparam TSubmissionMode Submission mode (`Instanced` or `NonInstanced`).
         * @param viewportHandle Target viewport.
         * @param context Visibility context to append.
         */
        template<typename TSubmissionMode>
        void addVisibleMember(const ViewportHandle viewportHandle, SceneMemberVisibilityContext<TMemberHandle, TSubmissionMode>&& context) {

            const auto idx = viewportHandle.entityId;

            if constexpr (std::is_same_v<TSubmissionMode, Instanced>) {
                if (visibilityContextsInstanced_.size() <= idx) {
                    visibilityContextsInstanced_.resize(idx + 1);
                }

                visibilityContextsInstanced_[idx].emplace_back(std::move(context));
            } else if constexpr (std::is_same_v<TSubmissionMode, NonInstanced>) {
                if (visibilityContextsNonInstanced_.size() <= idx) {
                    visibilityContextsNonInstanced_.resize(idx + 1);
                }

                visibilityContextsNonInstanced_[idx].emplace_back(std::move(context));
            } else {
                static_assert(always_false_v<TSubmissionMode>, "Unsupported submission mode");
            }

        }

        /**
         * @brief Returns culled member contexts for one viewport.
         * @tparam TSubmissionMode Submission mode (`Instanced` or `NonInstanced`).
         * @param viewportHandle Viewport to query.
         */
        template<typename TSubmissionMode>
        [[nodiscard]] std::span<SceneMemberVisibilityContext<TMemberHandle, TSubmissionMode>> culledMembers(const ViewportHandle viewportHandle = {})  {
            const auto idx = viewportHandle.entityId;
            if constexpr (std::is_same_v<TSubmissionMode, NonInstanced>) {
                 if (idx >= culledContextsNonInstanced_.size()) {
                    return {};
                }
                return culledContextsNonInstanced_[idx];
            } else if constexpr (std::is_same_v<TSubmissionMode, Instanced>) {
                 if (idx >= culledContextsInstanced_.size()) {
                    return {};
                }
                return culledContextsInstanced_[idx];
            } else {
                static_assert(always_false_v<TSubmissionMode>, "Unsupported submission mode");
            }
            return {};
        }

        /**
         * @brief Returns culled member context buckets for all viewports.
         * @tparam TSubmissionMode Submission mode (`Instanced` or `NonInstanced`).
         */
        template<typename TSubmissionMode>
        [[nodiscard]] std::span<std::vector<SceneMemberVisibilityContext<TMemberHandle, TSubmissionMode>>> culledMembers() const {
            if constexpr (std::is_same_v<TSubmissionMode, NonInstanced>) {
                return culledContextsNonInstanced_;
            } else if constexpr (std::is_same_v<TSubmissionMode, Instanced>) {
                return culledContextsInstanced_;
            } else {
                static_assert(always_false_v<TSubmissionMode>, "Unsupported submission mode");
            }
            return {};
        }

        /**
         * @brief Returns visible member contexts for one viewport.
         * @tparam TSubmissionMode Submission mode (`Instanced` or `NonInstanced`).
         * @param viewportHandle Viewport to query.
         */
        template<typename TSubmissionMode>
        [[nodiscard]] std::span<SceneMemberVisibilityContext<TMemberHandle, TSubmissionMode>> visibleMembers(const ViewportHandle viewportHandle) {
            const auto idx = viewportHandle.entityId;

            if constexpr (std::is_same_v<TSubmissionMode, NonInstanced>) {
                if (idx >= visibilityContextsNonInstanced_.size()) {
                    return {};
                }
                return visibilityContextsNonInstanced_[idx];
            } else if constexpr (std::is_same_v<TSubmissionMode, Instanced>) {
                if (idx >= visibilityContextsInstanced_.size()) {
                    return {};
                }
                return visibilityContextsInstanced_[idx];
            } else {
                static_assert(always_false_v<TSubmissionMode>, "Unsupported submission mode");
            }
            return {};
        }

        /**
         * @brief Returns visible member context buckets for all viewports.
         * @tparam TSubmissionMode Submission mode (`Instanced` or `NonInstanced`).
         */
        template<typename TSubmissionMode>
        [[nodiscard]] std::span<std::vector<SceneMemberVisibilityContext<TMemberHandle, TSubmissionMode>>> visibleMembers() {
            if constexpr (std::is_same_v<TSubmissionMode, NonInstanced>) {
                return visibilityContextsNonInstanced_;
            } else if constexpr (std::is_same_v<TSubmissionMode, Instanced>) {
                return visibilityContextsInstanced_;
            } else {
                static_assert(always_false_v<TSubmissionMode>, "Unsupported submission mode");
            }
            return {};
        }

        /**
         * @brief Clears all accumulated render and visibility state.
         */
        void clear () {
            sceneRenderContexts_.clear();
            for (auto& vec : visibilityContextsInstanced_) {
                vec.clear();
            }
            for (auto& vec : visibilityContextsNonInstanced_) {
                vec.clear();
            }
            for (auto& vec : culledContextsInstanced_) {
                vec.clear();
            }
            for (auto& vec : culledContextsNonInstanced_) {
                vec.clear();
            }
        }
    };


}