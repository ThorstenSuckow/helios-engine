/**
 * @file SceneMemberVisibilitySystem.ixx
 * @brief Builds per-frame scene-member visibility data per viewport.
 */
module;

#include <concepts>
#include <cassert>
#include <optional>
#include <tuple>
#include <vector>

#include "helios-engine-config.h"

export module helios.engine.scene.systems.SceneMemberVisibilitySystem;

import helios.engine.rendering.viewport.concepts.IsViewportHandle;

import helios.engine.scene.SceneMemberVisibilityRegistry;
import helios.engine.scene.types;
import helios.engine.scene.components;
import helios.engine.scene.concepts.IsFrustumCullerLike;

import helios.engine.rendering.common.components;
import helios.engine.rendering.common.commands;
import helios.engine.rendering.common.types;

import helios.engine.rendering.renderTarget.components.RenderTargetBindingComponent;

import helios.engine.spatial.components;

import helios.engine.rendering.viewport.types;
import helios.engine.rendering.viewport.ViewportEntity;

import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.types;
import helios.engine.runtime.concepts;
import helios.ecs.command.NullCommandBuffer;
import helios.ecs.common.concepts;
import helios.ecs.system.tags;

import helios.ecs.component;


import helios.core.log;

import helios.math;
import helios.engine.core.types;

using namespace helios::engine::core::types;
using namespace helios::engine::scene;
using namespace helios::engine::scene::types;
using namespace helios::engine::scene::concepts;
using namespace helios::engine::scene::components;
using namespace helios::ecs::components;
using namespace helios::engine::rendering::common::components;
using namespace helios::engine::rendering::viewport::concepts;
using namespace helios::engine::rendering::viewport;
using namespace helios::engine::rendering::viewport::types;
using namespace helios::engine::rendering::renderTarget::components;
using namespace helios::engine::scene::types;
using namespace helios::ecs::common::concepts;
using namespace helios::engine::spatial::components;
using namespace helios::engine::rendering::common::commands;
using namespace helios::engine::rendering::common::types;
using namespace helios::engine::rendering::common::components;
using namespace helios::ecs;
using namespace helios::engine::runtime::world;

#define HELIOS_LOG_SCOPE "helios::engine::scene::systems::SceneRenderSystem"
export namespace helios::engine::scene::systems {


    /**
     * @brief Computes visibility for scene members and stores the frame snapshot.
     *
     * This system is responsible for filling `SceneMemberVisibilityRegistry`
     * once per frame. The resulting visible/culled member lists are consumed by
     * subsequent rendering steps (for example by scene render submission).
     *
     * @tparam TMemberHandle Scene member handle type.
     * @tparam TSubmissionMode Submission mode (`Instanced` oder `NonInstanced`).
     * @tparam TCullingStrategy Strategy used to decide member visibility.
     * @tparam TUpdateContextType Type of the UpdateContext to use.
     */
    template<
        typename TMemberHandle,
        typename TSubmissionMode,
        typename TCullingStrategy,
        typename TUpdateContextType = helios::engine::runtime::world::types::SystemUpdateContext
    >
    requires IsFrustumCullerLike<TCullingStrategy, typename TCullingStrategy::MemberHandle_type> &&
             std::same_as<typename TCullingStrategy::MemberHandle_type, TMemberHandle> &&
            (std::same_as<TSubmissionMode, Instanced> || std::same_as<TSubmissionMode, NonInstanced>) &&
             runtime::concepts::ProvidesUpdateContext<TUpdateContextType, UpdateContext>
    class SceneMemberVisibilitySystem {

        /**
         * @brief Culling strategy used to decide member visibility per viewport.
         */
        TCullingStrategy cullingStrategy_;

        static inline auto& logger_ = helios::core::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);

        /**
         * @brief Tracks visible and culled members per viewport for diagnostics/debugging.
         */
        SceneMemberVisibilityRegistry<TMemberHandle, TSubmissionMode>& visibilityRegistry_;


        /**
         * @brief Evaluates all members for one submission mode and one viewport.
         *
         * Appends each member to the visibility registry as either visible or culled.
         *
         * @param updateContext ECS/world update context.
         * @param cullingContext Reusable culling context used per member.
         * @param sceneHandle Scene currently bound to the viewport.
         * @param renderTargetBindingComponent Render target bound to the viewport.
         * @param viewportEntity Viewport entity being processed.
         */
        void processMembers(
            UpdateContext& updateContext,
            CullingContext<TMemberHandle>& cullingContext,
            const SceneHandle sceneHandle,
            const RenderTargetBindingComponent<ViewportHandle>& renderTargetBindingComponent,
            const ViewportEntity &viewportEntity
        ) {

            for (auto [
                memberEntity,
                smc,
                rpc,
                transformWorld,
                boundsWorld
                ] : updateContext.view<
                TMemberHandle,
                SceneMemberComponent<TMemberHandle>,
                RenderPrototypeComponent<TMemberHandle, TSubmissionMode>,
                TransformComponent<TMemberHandle, World>,
                BoundsComponent<TMemberHandle, World>
            >().withActive()) {

                cullingContext.bounds = boundsWorld->value();
                cullingContext.handle = memberEntity.handle();

                auto memberContext = SceneMemberVisibilityContext<TMemberHandle, TSubmissionMode>{
                    memberEntity.handle(),
                    renderTargetBindingComponent.targetHandle(),
                    viewportEntity.handle(),
                    sceneHandle,
                    transformWorld->value()
                };

                if (smc->targetHandle() == sceneHandle && cullingStrategy_.shouldRender(cullingContext)) {
                    visibilityRegistry_.addVisibleMember(viewportEntity.handle(), std::move(memberContext));
                } else {
                    visibilityRegistry_.addCulledMember(viewportEntity.handle(), std::move(memberContext));
                }
            }
        }



    public:

        /**
         * @brief Runtime role tag used for engine system registration.
         */
        using EcsRoleTag = ecs::system::tags::TypedSystemRole;
        using UpdateContextType = TUpdateContextType;


        /**
         * @brief Creates the visibility system with strategy and output registry.
         * @param cullingStrategy Culling strategy instance.
         * @param visibilityRegistry Registry receiving per-frame visibility results.
         */
        explicit SceneMemberVisibilitySystem(TCullingStrategy cullingStrategy, SceneMemberVisibilityRegistry<TMemberHandle, TSubmissionMode>& visibilityRegistry)
        : cullingStrategy_(std::move(cullingStrategy)), visibilityRegistry_(visibilityRegistry) {
        }


        /**
         * @brief Rebuilds the visibility registry for the current frame.
         *
         * The registry is cleared first, then all active viewport bindings are
         * traversed. For each viewport, members are tested and classified into
         * visible/culled buckets per submission mode.
         *
         * @param updateCtx ECS/world update context.
         */
        bool update(TUpdateContextType& updateCtx) noexcept {

            auto& updateContext = updateCtx.updateContext();

            visibilityRegistry_.clear();

            for (auto [viewportEntity, renderTargetBindingComponent, sbc, cbc] : updateContext.template view<
                ViewportHandle,
                RenderTargetBindingComponent<ViewportHandle>,
                SceneBindingComponent<ViewportHandle>,
                CameraBindingComponent<ViewportHandle>
            >().withActive()) {

                const auto sceneHandle  = sbc->targetHandle();
                const auto cameraHandle = cbc->targetHandle();

                const auto camera = updateContext.template find<CameraHandle>(cameraHandle);
                if (!camera) {
                    assert(false && "Camera not found");
                    logger_.error("Camera not found");
                    continue;
                }
                auto* pmc = camera->template get<ProjectionMatrixComponent<CameraHandle>>();
                if (!pmc) {
                    assert(pmc && "Camera had no ProjectionMatrixComponent");
                    logger_.error("Camera had no ProjectionMatrixComponent");
                    continue;
                }
                auto* lac = camera->template get<ViewMatrixComponent<CameraHandle>>();
                if (!lac) {
                    assert(lac && "Camera had no ViewMatrixComponent");
                    logger_.error("Camera had no ViewMatrixComponent");
                    continue;
                }

                auto* pcc = camera->template get<PerspectiveCameraComponent<CameraHandle>>();
                auto frustumPlanes = helios::math::frustumPlanes(
                    pcc->fovY(), pcc->aspectRatio(), pcc->zNear(), pcc->zFar(), lac->value()
                );

                /**
                 * @todo  Frustum culling only if camera changed amd if objects are stationary?
                 */
                auto cullingContext = CullingContext<TMemberHandle>{frustumPlanes, pmc->value(), lac->value()};

                /**
                 * Moved from processMembers() to make sure a scene gets re-rendered (i.e. framebuffer cleared)
                 * even if no members are visible. Otherwise the last frame's contents would remain in the framebuffer.
                 */
                visibilityRegistry_.addSceneRenderContext({
                    renderTargetBindingComponent->targetHandle(), viewportEntity.handle(), sceneHandle
                });

                processMembers(
                    updateContext, cullingContext, sceneHandle, *renderTargetBindingComponent, viewportEntity);
            }
            return true;
        }


    };

}