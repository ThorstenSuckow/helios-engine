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

import helios.engine.scene.SceneMemberVisibilityRegistry;
import helios.engine.scene.types;
import helios.engine.scene.components;
import helios.engine.scene.concepts.IsFrustumCullerLike;

import helios.engine.rendering.common.components;
import helios.engine.rendering.common.commands;
import helios.engine.rendering.common.types;

import helios.engine.rendering.common.types;

import helios.engine.rendering.common.components.RenderTargetBindingComponent;

import helios.engine.spatial.components;

import helios.engine.runtime.gameloop.types;

import helios.ecs.command.types;
import helios.ecs;


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
using namespace helios::engine::scene::types;
using namespace helios::ecs::common::concepts;
using namespace helios::engine::spatial::components;
using namespace helios::engine::rendering::common::commands;
using namespace helios::engine::rendering::common::types;
using namespace helios::engine::rendering::common::components;
using namespace helios::ecs;


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
     */
    template<
        typename TMemberHandle,
        typename TSubmissionMode,
        typename TRenderHandles
    >
    requires (std::is_same_v<TSubmissionMode, Instanced> || std::is_same_v<TSubmissionMode, NonInstanced>)
    class SceneRenderSystem {

        using UpdateContext = engine::runtime::gameloop::types::UpdateContext;

        using SceneMemberVisibilityRegistry = SceneMemberVisibilityRegistry<TMemberHandle, TSubmissionMode, TRenderHandles>;
        using SceneMemberVisibilityContext = SceneMemberVisibilityContext<TMemberHandle, TSubmissionMode, TRenderHandles>;
        using RenderSceneMemberCommand = RenderSceneMemberCommand<TMemberHandle, TRenderHandles>;
        using SceneMemberRenderContext = SceneMemberRenderContext<TMemberHandle, TRenderHandles>;
        using RenderInstanceBatchCommand = RenderInstanceBatchCommand<TMemberHandle, TRenderHandles>;
        using RenderInstanceBatchContext = InstanceRenderBatchContext<TMemberHandle, TRenderHandles>;
        using RenderSceneCommand = RenderSceneCommand<TMemberHandle, TRenderHandles>;
        using RenderPrototypeComponent = RenderPrototypeComponent<TMemberHandle, TSubmissionMode, TRenderHandles>;

        using CommandBuffer = ecs::command::TypedCommandBuffer<
            RenderSceneCommand,
            RenderSceneMemberCommand,
            RenderInstanceBatchCommand
        >;

        static inline auto& logger_ = helios::core::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);



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
             std::span<const std::vector<SceneMemberVisibilityContext>> visibilityContexts,
             CommandBuffer& cmdBuffer) requires std::is_same_v<TSubmissionMode, NonInstanced>  {


            for (const auto& viewportContexts : visibilityContexts) {

                for (const auto& memberContext : viewportContexts) {
                    const auto sceneHandle = memberContext.sceneHandle;
                    const auto renderTargetHandle = memberContext.renderTargetHandle;
                    const auto viewportHandle = memberContext.viewportHandle;

                    const auto entity = updateContext.find<TMemberHandle>(memberContext.memberHandle);
                    assert(entity && "Unexpected missing entity");

                    const auto* renderPrototype = entity->template get<RenderPrototypeComponent>();
                    assert(renderPrototype && "Unexpected missing RenderPrototypeComponent");

                    cmdBuffer.template add<RenderSceneMemberCommand>(SceneMemberRenderContext{
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
            std::span<const std::vector<SceneMemberVisibilityContext>> visibilityContexts,
            CommandBuffer& cmdBuffer) requires std::is_same_v<TSubmissionMode, Instanced> {

            std::optional<RenderInstanceBatchContext> renderBatchContext;

            auto flushCurrentBatch = [&]() {
                if (!renderBatchContext || renderBatchContext->memberHandles.empty()) {
                    renderBatchContext.reset();
                    return;
                }

                cmdBuffer.template add<RenderInstanceBatchCommand>(
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

                    const auto* renderPrototype = entity->template get<RenderPrototypeComponent>();
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

                    renderBatchContext->memberHandles.push_back(
                        memberContext.memberHandle
                    );
                    renderBatchContext->instanceData.push_back({
                        memberContext.worldMatrix, memberContext.normalizedAge
                    });
                }
            }

            flushCurrentBatch();
        }



    public:


        SceneRenderSystem() = default;

        /**
         * @brief Dispatches render commands from the current visibility snapshot.
         *
         * Emits one `RenderSceneCommand` per scene/render-target/viewport context,
         * then emits member commands for instanced and non-instanced visible
         * members stored in `SceneMemberVisibilityRegistry`.
         *
         * @param updateContext Current frame update context.
         * @param cmdBuffer Command buffer receiving extracted render commands.
         */
        void update(UpdateContext& updateContext, CommandBuffer& cmdBuffer, const SceneMemberVisibilityRegistry& visibilityRegistry) noexcept {

            for (auto sceneRenderContexts = visibilityRegistry.sceneRenderContexts();
                auto& sceneRenderContext : sceneRenderContexts) {
                cmdBuffer.template add<RenderSceneCommand>(
                    sceneRenderContext
                );
            }

            const auto members = visibilityRegistry.visibleMembers();

            if constexpr (std::is_same_v<TSubmissionMode, Instanced>) {
                dispatchInstancedRenderCommands(updateContext, members, cmdBuffer);
            } else if constexpr (std::is_same_v<TSubmissionMode, NonInstanced>) {
                dispatchNonInstancedRenderCommands(updateContext, members, cmdBuffer);
            } else {
                static_assert(false, "Unsupported submission mode");
            }
        }

    };

}