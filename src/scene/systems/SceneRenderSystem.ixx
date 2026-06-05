/**
 * @file SceneRenderSystem.ixx
 * @brief Extracts visible scene members into render commands per viewport.
 */
module;

#include <concepts>
#include <cassert>
#include <tuple>
#include "helios-engine-config.h"

export module helios.engine.scene.systems.SceneRenderSystem;

import helios.engine.rendering.viewport.concepts.IsViewportHandle;

import helios.engine.scene.SceneMemberVisibilityRegistry;
import helios.engine.scene.types;
import helios.engine.scene.components;
import helios.engine.scene.concepts.IsFrustumCullerLike;

import helios.engine.rendering.common.components;
import helios.engine.rendering.common.commands;

import helios.engine.rendering.renderTarget.components.RenderTargetBindingComponent;

import helios.engine.spatial.components;

import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.messaging.command.NullCommandBuffer;
import helios.engine.runtime.messaging.command.concepts.IsCommandBufferLike;
import helios.engine.runtime.world.tags.SystemRole;

import helios.ecs.components.Active;


import helios.engine.util.log;

import helios.math;
import helios.engine.core.types.ComponentTypeTags;

using namespace helios::engine::core::types;
using namespace helios::engine::scene;
using namespace helios::engine::scene::types;
using namespace helios::engine::scene::concepts;
using namespace helios::engine::scene::components;
using namespace helios::ecs::components;
using namespace helios::engine::rendering::common::components;
using namespace helios::engine::rendering::viewport::concepts;
using namespace helios::engine::rendering::renderTarget::components;
using namespace helios::engine::scene::types;
using namespace helios::engine::runtime::messaging::command::concepts;
using namespace helios::engine::spatial::components;
using namespace helios::engine::rendering::common::commands;
using namespace helios::engine::rendering::common::components;
using namespace helios::engine::runtime::messaging::command;

#define HELIOS_LOG_SCOPE "helios::engine::scene::systems::SceneRenderSystem"
export namespace helios::engine::scene::systems {

    /**
     * @brief System that extracts per-member render contexts for active viewports.
     *
     * @details For each active viewport binding, the system resolves the bound
     * scene and camera, evaluates the configured culling strategy for scene
     * members, and writes accepted members as `RenderSceneMemberCommand`s into the target
     * command buffer.
     *
     * @tparam TOwnerHandle Viewport entity handle type.
     * @tparam TMemberHandle Scene member handle type.
     * @tparam TCullingStrategy Frustum-culling strategy type.
     * @tparam TCommandBuffer Command buffer used for extracted render commands.
     */
    template<
        typename TOwnerHandle,
        typename TMemberHandle,
        typename TCullingStrategy,
        typename TCommandBuffer = NullCommandBuffer
    >
    requires IsViewportHandle<TOwnerHandle> &&
             IsFrustumCullerLike<TCullingStrategy, typename TCullingStrategy::MemberHandle_type> &&
             std::same_as<typename TCullingStrategy::MemberHandle_type, TMemberHandle> &&
             IsCommandBufferLike<TCommandBuffer>
    class SceneRenderSystem {

        TCullingStrategy cullingStrategy_;

        static inline auto& logger_ = helios::engine::util::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);

        SceneMemberVisibilityRegistry<TMemberHandle>& visibilityRegistry_;
    public:

        /** @brief Runtime role tag used for engine system registration. */
        using EngineRoleTag = helios::engine::runtime::world::tags::SystemRole;

        /** @brief Command buffer type used by this extraction system. */
        using CommandBuffer_type = TCommandBuffer;

        /**
         * @brief Constructs the system with a culling strategy instance.
         *
         * @param cullingStrategy Strategy used to decide whether a scene member
         * should produce render work.
         * @param visibilityRegistry Registry for tracking culled and visible handles per viewport.
         */
        explicit SceneRenderSystem(TCullingStrategy cullingStrategy, SceneMemberVisibilityRegistry<TMemberHandle>& visibilityRegistry)
        : cullingStrategy_(std::move(cullingStrategy)), visibilityRegistry_(visibilityRegistry) {}

        /**
         * @brief Extracts render commands for active viewports.
         *
         * @param updateContext Current frame update context.
         * @param cmdBuffer Command buffer receiving extracted render commands.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext, TCommandBuffer& cmdBuffer) noexcept {


            for (auto [viewportEntity, fbc, sbc, cbc, viewportActive] : updateContext.view<
                TOwnerHandle,
                RenderTargetBindingComponent<TOwnerHandle>,
                SceneBindingComponent<TOwnerHandle>,
                CameraBindingComponent<TOwnerHandle>,
                Active<TOwnerHandle>
            >().whereEnabled()) {

                const auto sceneHandle  = sbc->targetHandle();
                const auto cameraHandle = cbc->targetHandle();

                const auto camera = updateContext.find<TMemberHandle>(cameraHandle);
                if (!camera) {
                    assert(false && "Camera not found");
                    logger_.error("Camera not found");
                    continue;
                }
                auto* pmc = camera->template get<ProjectionMatrixComponent<TMemberHandle>>();
                if (!pmc) {
                    assert(pmc && "Camera had no ProjectionMatrixComponent");
                    logger_.error("Camera had no ProjectionMatrixComponent");
                    continue;
                }
                auto* lac = camera->template get<ViewMatrixComponent<TMemberHandle>>();
                if (!lac) {
                    assert(lac && "Camera had no ViewMatrixComponent");
                    logger_.error("Camera had no ViewMatrixComponent");
                    continue;
                }

                auto* pcc = camera->template get<PerspectiveCameraComponent<TMemberHandle>>();
                auto frustumPlanes = helios::math::frustumPlanes(
                    pcc->fovY(), pcc->aspectRatio(), pcc->zNear(), pcc->zFar(), lac->value()
                );

                auto cullingContext = CullingContext<TMemberHandle>{frustumPlanes, pmc->value(), lac->value()};

                cmdBuffer.template add<RenderSceneCommand<TMemberHandle>>(
                   SceneRenderContext<TMemberHandle>{
                       fbc->targetHandle(),
                       viewportEntity.handle(),
                       sceneHandle
                   });



#if HELIOS_DEBUG
                std::size_t available = 0;
                std::size_t used      = 0;
#endif

                for (auto [
                    memberEntity,
                    smc,
                    rpc,
                    transformWorld,
                    boundsWorld,
                    memberActive
                    ] : updateContext.view<
                    TMemberHandle,
                    SceneMemberComponent<TMemberHandle>,
                    RenderPrototypeComponent<TMemberHandle>,
                    TransformComponent<TMemberHandle, World>,
                    BoundsComponent<TMemberHandle, World>,
                    Active<TMemberHandle>
                >().whereEnabled()) {

                    cullingContext.bounds = boundsWorld->value();
                    cullingContext.handle = memberEntity.handle();

#if HELIOS_DEBUG
                    available++;
#endif
                    if (smc->targetHandle() == sceneHandle && cullingStrategy_.shouldRender(cullingContext)) {
#if HELIOS_DEBUG
                        used++;
#endif
                        std::ignore = visibilityRegistry_.addVisibleMember(viewportEntity.handle(), memberEntity.handle());
                        cmdBuffer.template add<RenderSceneMemberCommand<TMemberHandle>>(
                            SceneMemberRenderContext<TMemberHandle>{
                                memberEntity.handle(),
                                fbc->targetHandle(),
                                viewportEntity.handle(),
                                sceneHandle,
                                rpc->meshHandle(),
                                rpc->materialHandle(),
                                rpc->shaderHandle(),
                                transformWorld->value()
                            });


                    } else {
                        std::ignore = visibilityRegistry_.addCulledMember(viewportEntity.handle(), memberEntity.handle());
                    }
                }

#if HELIOS_DEBUG
                logger_.debug("Available {0}, culled in {1}, used {2} scene members for viewport {3}.", available, available - used, used, viewportEntity.handle().entityId);
#endif

            }

        }

    };

}