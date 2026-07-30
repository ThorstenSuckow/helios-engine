/**
 * @file SceneMemberVisibilityRegistry.ixx
 * @brief Registry for visible and culled scene members grouped by viewport and submission mode.
 */
module;

#include "helios-engine-config.h"
#include <span>
#include <cassert>
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
     * @tparam TSubmissionMode Submission mode (`Instanced` oder `NonInstanced`).
     */
    template<typename TMemberHandle, typename TSubmissionMode>
    class SceneMemberVisibilityRegistry {


        struct RenderTargetViewportSceneKey {
            std::uint32_t a;
            std::uint32_t b;
            std::uint32_t c;

            friend bool operator==(const RenderTargetViewportSceneKey&, const RenderTargetViewportSceneKey&) = default;
        };

        std::vector<RenderTargetViewportSceneKey> renderTargetViewportSceneKeys_;

        std::vector<std::vector<SceneMemberVisibilityContext<TMemberHandle, TSubmissionMode>>> visibilityContexts_;
        std::vector<std::vector<SceneMemberVisibilityContext<TMemberHandle, TSubmissionMode>>> culledContexts_;

        std::vector<SceneRenderContext<TMemberHandle>> sceneRenderContexts_;


    public:

        /**
         * @brief Constructs the registry with pre-reserved top-level storage.
         */
        SceneMemberVisibilityRegistry() {
            visibilityContexts_.reserve(DEFAULT_VIEWPORT_POOL_CAPACITY);
            for (auto& vec : visibilityContexts_) {
                vec.reserve(HELIOS_DEFAULT_VISIBLE_MEMBER_CAPACITY);
            }
            culledContexts_.reserve(DEFAULT_VIEWPORT_POOL_CAPACITY);
            for (auto& vec : culledContexts_) {
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
        void addCulledMember(const ViewportHandle viewportHandle, SceneMemberVisibilityContext<TMemberHandle, TSubmissionMode>&& context) {
            const auto idx = viewportHandle.entityId;
            if (culledContexts_.size() <= idx) {
                culledContexts_.resize(idx + 1);
            }
            culledContexts_[idx].emplace_back(std::move(context));
            
        }


        /**
         * @brief Adds a visible member visibility context for a viewport.
         * @tparam TSubmissionMode Submission mode (`Instanced` or `NonInstanced`).
         * @param viewportHandle Target viewport.
         * @param context Visibility context to append.
         */
        void addVisibleMember(const ViewportHandle viewportHandle, SceneMemberVisibilityContext<TMemberHandle, TSubmissionMode>&& context) {
            const auto idx = viewportHandle.entityId;
            if (visibilityContexts_.size() <= idx) {
                visibilityContexts_.resize(idx + 1);
            }

            visibilityContexts_[idx].emplace_back(std::move(context));
        }

        /**
         * @brief Returns culled member contexts for one viewport.
         * @tparam TSubmissionMode Submission mode (`Instanced` or `NonInstanced`).
         * @param viewportHandle Viewport to query.
         */
        [[nodiscard]] std::span<SceneMemberVisibilityContext<TMemberHandle, TSubmissionMode>> culledMembers(const ViewportHandle viewportHandle = {})  {
            const auto idx = viewportHandle.entityId;
            if (idx >= culledContexts_.size()) {
                return {};
            }
            return culledContexts_[idx];
        }

        /**
         * @brief Returns culled member context buckets for all viewports.
         * @tparam TSubmissionMode Submission mode (`Instanced` or `NonInstanced`).
         */
        [[nodiscard]] std::span<std::vector<SceneMemberVisibilityContext<TMemberHandle, TSubmissionMode>>> culledMembers() const {
            return culledContexts_;
        }

        /**
         * @brief Returns visible member contexts for one viewport.
         * @tparam TSubmissionMode Submission mode (`Instanced` or `NonInstanced`).
         * @param viewportHandle Viewport to query.
         */
        [[nodiscard]] std::span<const SceneMemberVisibilityContext<TMemberHandle, TSubmissionMode>> visibleMembers(const ViewportHandle viewportHandle) const noexcept {
            const auto idx = viewportHandle.entityId;
            if (idx >= visibilityContexts_.size()) {
                return {};
            }
            return visibilityContexts_[idx];
        }

        /**
         * @brief Returns visible member context buckets for all viewports.
         * @tparam TSubmissionMode Submission mode (`Instanced` or `NonInstanced`).
         */
        [[nodiscard]] std::span<const std::vector<SceneMemberVisibilityContext<TMemberHandle, TSubmissionMode>>> visibleMembers() const noexcept {
            return visibilityContexts_;
        }

        /**
         * @brief Clears all accumulated render and visibility state.
         */
        void clear () {
            sceneRenderContexts_.clear();
            for (auto& vec : visibilityContexts_) {
                vec.clear();
            }
            for (auto& vec : culledContexts_) {
                vec.clear();
            }

        }
    };


}