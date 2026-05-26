/**
 * @file SceneRenderExtractionSystem.ixx
 * @brief Extracts visible scene members into render commands per viewport.
 */
module;

#include <concepts>
#include <cassert>

export module helios.engine.rendering.viewport.systems.SceneRenderExtractionSystem;

import helios.engine.rendering.viewport.concepts.IsViewportHandle;

import helios.engine.scene.types;
import helios.engine.scene.components;
import helios.engine.scene.concepts.IsFrustumCullerLike;

import helios.engine.rendering.common.components;
import helios.engine.rendering.common.commands.RenderCommand;

import helios.engine.rendering.framebuffer.components.FramebufferBindingComponent;

import helios.engine.spatial.components;

import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.messaging.command.NullCommandBuffer;
import helios.engine.runtime.messaging.command.concepts.IsCommandBufferLike;
import helios.engine.runtime.world.tags.SystemRole;

import helios.ecs.components.Active;


import helios.engine.util.log;

using namespace helios::engine::scene;
using namespace helios::engine::scene::types;
using namespace helios::engine::scene::concepts;
using namespace helios::engine::scene::components;
using namespace helios::ecs::components;
using namespace helios::engine::rendering::common::components;
using namespace helios::engine::rendering::viewport::concepts;
using namespace helios::engine::rendering::framebuffer::components;
using namespace helios::engine::scene::types;
using namespace helios::engine::runtime::messaging::command::concepts;
using namespace helios::engine::spatial::components;
using namespace helios::engine::rendering::common::commands;
using namespace helios::engine::rendering::common::components;
using namespace helios::engine::runtime::messaging::command;

#define HELIOS_LOG_SCOPE "helios::engine::rendering::viewport::systems::SceneRenderExtractionSystem"
export namespace helios::engine::rendering::viewport::systems {

    /**
     * @brief System that extracts per-member render contexts for active viewports.
     *
     * @details For each active viewport binding, the system resolves the bound
     * scene and camera, evaluates the configured culling strategy for scene
     * members, and writes accepted members as `RenderCommand`s into the target
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
    class SceneRenderExtractionSystem {

        TCullingStrategy cullingStrategy_;

        static inline auto& logger_ = helios::engine::util::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);

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
         */
        explicit SceneRenderExtractionSystem(TCullingStrategy cullingStrategy = TCullingStrategy())
        : cullingStrategy_(std::move(cullingStrategy)) {}

        /**
         * @brief Extracts render commands for active viewports.
         *
         * @param updateContext Current frame update context.
         * @param cmdBuffer Command buffer receiving extracted render commands.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext, TCommandBuffer& cmdBuffer) noexcept {


            for (auto [viewportEntity, fbc, sbc, cbc, viewportActive] : updateContext.view<
                TOwnerHandle,
                FramebufferBindingComponent<TOwnerHandle>,
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
                auto* pcc = camera->template get<ProjectionMatrixComponent<TMemberHandle>>();
                if (!pcc) {
                    assert(pcc && "Camera had no PerspectiveCameraComponent");
                    logger_.error("Camera had no PerspectiveCameraComponent");
                    continue;
                }
                auto* lac = camera->template get<ViewMatrixComponent<TMemberHandle>>();
                if (!lac) {
                    assert(lac && "Camera had no ViewMatrixComponent");
                    logger_.error("Camera had no ViewMatrixComponent");
                    continue;
                }

                for (auto [
                    memberEntity,
                    smc,
                    rpc,
                    wmc,
                    wbc,
                    memberActive
                    ] : updateContext.view<
                    TMemberHandle,
                    SceneMemberComponent<TMemberHandle>,
                    RenderPrototypeComponent<TMemberHandle>,
                    WorldMatrixComponent<TMemberHandle>,
                    WorldBoundsComponent<TMemberHandle>,
                    Active<TMemberHandle>
                >().whereEnabled()) {

                    if (smc->targetHandle() == sceneHandle && cullingStrategy_.shouldRender(
                            CullingContext{pcc->value(), lac->value(), wbc->value(), memberEntity.handle()}
                        )
                    ) {

                        cmdBuffer.template add<RenderCommand<TMemberHandle>>(
                            SceneMemberRenderContext<TMemberHandle>{
                                memberEntity.handle(),
                                fbc->targetHandle(),
                                viewportEntity.handle(),
                                sceneHandle,
                                rpc->meshHandle(),
                                rpc->materialHandle(),
                                rpc->shaderHandle(),
                                wmc->value()
                            });


                    }
                }
            }

        }

    };

}