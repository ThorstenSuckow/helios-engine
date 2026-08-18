/**
 * @file SceneRenderSystem.ixx
 * @brief Emits scene render commands from per-frame visibility snapshots.
 */
module;

#include <concepts>
#include <cassert>
#include <optional>
#include <span>
#include <vector>

#include "helios-engine-config.h"

export module helios.engine.scene.systems.SceneRenderSystem;

import helios.engine.rendering.viewport.concepts.IsViewportHandle;

import helios.engine.scene.SceneMemberVisibilityRegistry;
import helios.engine.scene.types;
import helios.engine.scene.components;
import helios.engine.scene.concepts.IsFrustumCullerLike;

import helios.engine.rendering.common.components;
import helios.engine.rendering.common.commands;
import helios.engine.rendering.common.types;

import helios.engine.rendering.renderTarget.types;
import helios.engine.rendering.viewport.types;

import helios.engine.rendering.renderTarget.components.RenderTargetBindingComponent;

import helios.engine.spatial.components;

import helios.engine.rendering.viewport.ViewportEntity;

import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.types;
import helios.engine.runtime.concepts;
import helios.ecs;
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
using namespace helios::engine::rendering::renderTarget::components;
using namespace helios::engine::scene::types;
using namespace helios::ecs::common::concepts;
using namespace helios::engine::spatial::components;
using namespace helios::engine::rendering::common::commands;
using namespace helios::engine::rendering::common::types;
using namespace helios::engine::rendering::common::components;
using namespace helios::ecs;
using namespace helios::engine::runtime::world;
using namespace helios::engine::rendering::renderTarget::types;
using namespace helios::engine::rendering::viewport::types;

#define HELIOS_LOG_SCOPE "helios::engine::scene::systems::SceneRenderSystem"
export namespace helios::engine::scene::systems {

    /**
     * @brief System that dispatches render commands from visibility results.
     *
     * @details This system consumes the frame-local `SceneMemberVisibilityRegistry`
     * produced earlier by `SceneMemberVisibilitySystem`. It emits scene-level
     * render commands and member-level commands for both instanced and
     * non-instanced submission paths.
     *
     * @tparam TMemberHandle Scene member handle type.
     * @tparam TSubmissionMode Submission mode (`Instanced` oder `NonInstanced`).
     * @tparam TCommandBuffer Command buffer used for extracted render commands.
     * @tparam TUpdateContextType Type of the UpdateContext to use.
     */
    template<
        typename TMemberHandle,
        typename TSubmissionMode,
        typename TCommandBuffer = ecs::command::NullCommandBuffer,
        typename TUpdateContextType = helios::engine::runtime::world::types::SystemUpdateContext
    >
    requires ecs::command::concepts::IsCommandBufferLike<TCommandBuffer> &&
            (std::is_same_v<TSubmissionMode, Instanced> || std::is_same_v<TSubmissionMode, NonInstanced>) &&
             runtime::concepts::ProvidesUpdateContext<TUpdateContextType, UpdateContext>
    class SceneRenderSystem {

        static inline auto& logger_ = helios::core::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);

        /**
         * @brief Central per-frame visibility snapshot consumed by this system.
         *
         * Filled by `SceneMemberVisibilitySystem` and read-only in this stage.
         */
        const SceneMemberVisibilityRegistry<TMemberHandle, TSubmissionMode>& visibilityRegistry_;


        /**
         * @brief Dispatches non-instanced member render commands.
         *
         * Iterates visible non-instanced member contexts and emits one
         * `RenderSceneMemberCommand` per member.
         *
         * @param updateContext Current frame update context.
         * @param visibilityContexts Visible non-instanced members grouped by viewport.
         * @param cmdBuffer Command buffer receiving render commands.
         */
        void dispatchNonInstancedRenderCommands(
            UpdateContext& updateContext,
             std::span<const std::vector<SceneMemberVisibilityContext<TMemberHandle, TSubmissionMode>>> visibilityContexts,
             TCommandBuffer& cmdBuffer) requires std::is_same_v<TSubmissionMode, NonInstanced>  {


            for (const auto& viewportContexts : visibilityContexts) {

                for (const auto& memberContext : viewportContexts) {
                    const auto sceneHandle = memberContext.sceneHandle;
                    const auto renderTargetHandle = memberContext.renderTargetHandle;
                    const auto viewportHandle = memberContext.viewportHandle;

                    const auto entity = updateContext.find<TMemberHandle>(memberContext.memberHandle);
                    assert(entity && "Unexpected missing entity");

                    const auto* renderPrototype = entity->template get<RenderPrototypeComponent<TMemberHandle, TSubmissionMode>>();
                    assert(renderPrototype && "Unexpected missing RenderPrototypeComponent");

                    cmdBuffer.template add<RenderSceneMemberCommand<TMemberHandle>>(SceneMemberRenderContext<TMemberHandle>{
                        memberContext.memberHandle,
                        renderTargetHandle,
                        viewportHandle,
                        sceneHandle,
                        renderPrototype->meshHandle(),
                        renderPrototype->textureHandle(),
                        renderPrototype->materialHandle(),
                        renderPrototype->shaderHandle(),
                        memberContext.worldMatrix
                    });
                }
            }
        }


        /**
         * @brief Dispatches instanced member render commands in batches.
         *
         * Builds `InstanceRenderBatchContext` groups with matching render state
         * and emits `RenderInstanceBatchCommand` whenever a batch boundary is hit.
         *
         * @param updateContext Current frame update context.
         * @param visibilityContexts Visible instanced members grouped by viewport.
         * @param cmdBuffer Command buffer receiving render commands.
         */
        void dispatchInstancedRenderCommands (
            UpdateContext& updateContext,
            std::span<const std::vector<SceneMemberVisibilityContext<TMemberHandle, TSubmissionMode>>> visibilityContexts,
            TCommandBuffer& cmdBuffer) requires std::is_same_v<TSubmissionMode, Instanced> {

            std::optional<InstanceRenderBatchContext<TMemberHandle>> renderBatchContext;

            auto flushCurrentBatch = [&]() {
                if (!renderBatchContext || renderBatchContext->memberHandles.empty()) {
                    renderBatchContext.reset();
                    return;
                }

                cmdBuffer.template add<RenderInstanceBatchCommand<TMemberHandle>>(
                    std::move(*renderBatchContext)
                );

                renderBatchContext.reset();
            };

            for (const auto& viewportContexts : visibilityContexts) {

                for (const auto& memberContext : viewportContexts) {
                    const auto sceneHandle = memberContext.sceneHandle;
                    const auto renderTargetHandle = memberContext.renderTargetHandle;
                    const auto viewportHandle = memberContext.viewportHandle;

                    const auto entity = updateContext.find<TMemberHandle>(memberContext.memberHandle);
                    assert(entity && "Unexpected missing entity");

                    const auto* renderPrototype = entity->template get<RenderPrototypeComponent<TMemberHandle, Instanced>>();
                    assert(renderPrototype && "Unexpected missing RenderPrototypeComponent");

                    if (!renderBatchContext ||
                        memberContext.renderTargetHandle != renderBatchContext->renderTargetHandle ||
                        memberContext.viewportHandle != renderBatchContext->viewportHandle ||
                        sceneHandle != renderBatchContext->sceneHandle ||
                        renderPrototype->meshHandle() != renderBatchContext->meshHandle ||
                        renderPrototype->materialHandle() != renderBatchContext->materialHandle ||
                        renderPrototype->textureHandle() != renderBatchContext->textureHandle ||
                        renderPrototype->shaderHandle() != renderBatchContext->shaderHandle) {

                            flushCurrentBatch();

                            renderBatchContext.emplace(
                                memberContext.renderTargetHandle,
                                viewportHandle,
                                sceneHandle,
                                renderPrototype->meshHandle(),
                                renderPrototype->textureHandle(),
                                renderPrototype->materialHandle(),
                                renderPrototype->shaderHandle()
                            );
                        }

                    renderBatchContext->memberHandles.push_back(memberContext.memberHandle);
                    renderBatchContext->instanceData.push_back({memberContext.worldMatrix});
                }
            }

            flushCurrentBatch();
        }



    public:

        /**
         * @brief Runtime role tag used for engine system registration.
         */
        using EcsRoleTag = ecs::system::tags::TypedSystemRole;

        /**
         * @brief Command buffer type used by this extraction system.
         */
        using CommandBuffer_type = TCommandBuffer;
        using UpdateContextType = TUpdateContextType;

        /**
         * @brief Constructs the system with the visibility snapshot registry.
         *
         * @param visibilityRegistry Registry containing per-frame visible/culled members.
         */
        explicit SceneRenderSystem(const SceneMemberVisibilityRegistry<TMemberHandle, TSubmissionMode>& visibilityRegistry)
        : visibilityRegistry_(visibilityRegistry) {
        }

        /**
         * @brief Dispatches render commands from the current visibility snapshot.
         *
         * Emits one `RenderSceneCommand` per scene/render-target/viewport context,
         * then emits member commands for instanced and non-instanced visible
         * members stored in `SceneMemberVisibilityRegistry`.
         *
         * @param updateCtx Current frame update context.
         * @param cmdBuffer Command buffer receiving extracted render commands.
         */
        bool update(TUpdateContextType& updateCtx, TCommandBuffer& cmdBuffer) noexcept {

            auto& updateContext = updateCtx.updateContext();

            for (auto sceneRenderContexts = visibilityRegistry_.sceneRenderContexts();
                auto& sceneRenderContext : sceneRenderContexts) {
                cmdBuffer.template add<RenderSceneCommand<TMemberHandle>>(sceneRenderContext);
            }

            const auto members = visibilityRegistry_.visibleMembers();

            if constexpr (std::is_same_v<TSubmissionMode, Instanced>) {
                dispatchInstancedRenderCommands(updateContext, members, cmdBuffer);
            } else if constexpr (std::is_same_v<TSubmissionMode, NonInstanced>) {
                dispatchNonInstancedRenderCommands(updateContext, members, cmdBuffer);
            } else {
                static_assert(false, "Unsupported submission mode");
            }
            return true;
        }

    };

}