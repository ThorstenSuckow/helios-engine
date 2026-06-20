/**
 * @file SceneRenderSystem.ixx
 * @brief Emits scene render commands from per-frame visibility snapshots.
 */
module;

#include <concepts>
#include <cassert>
#include <optional>
#include <tuple>
#include <vector>
#include <cstdint>

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
using namespace helios::engine::rendering::viewport;
using namespace helios::engine::rendering::renderTarget::components;
using namespace helios::engine::scene::types;
using namespace helios::engine::runtime::messaging::command::concepts;
using namespace helios::engine::spatial::components;
using namespace helios::engine::rendering::common::commands;
using namespace helios::engine::rendering::common::types;
using namespace helios::engine::rendering::common::components;
using namespace helios::engine::runtime::messaging::command;
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
     * @tparam TOwnerHandle Viewport entity handle type.
     * @tparam TMemberHandle Scene member handle type.
     * @tparam TCommandBuffer Command buffer used for extracted render commands.
     */
    template<
        typename TOwnerHandle,
        typename TMemberHandle,
        typename TCommandBuffer = NullCommandBuffer
    >
    requires IsViewportHandle<TOwnerHandle> &&
             IsCommandBufferLike<TCommandBuffer>
    class SceneRenderSystem {

        static inline auto& logger_ = helios::engine::util::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);

        /**
         * @brief Central per-frame visibility snapshot consumed by this system.
         *
         * Filled by `SceneMemberVisibilitySystem` and read-only in this stage.
         */
        SceneMemberVisibilityRegistry<TMemberHandle>& visibilityRegistry_;


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
             std::span<const std::vector<SceneMemberVisibilityContext<TMemberHandle, NonInstanced>>> visibilityContexts,
             TCommandBuffer& cmdBuffer) {


            for (const auto& viewportContexts : visibilityContexts) {

                for (const auto& memberContext : viewportContexts) {
                    const auto sceneHandle = memberContext.sceneHandle;
                    const auto renderTargetHandle = memberContext.renderTargetHandle;
                    const auto viewportHandle = memberContext.viewportHandle;

                    const auto entity = updateContext.find<TMemberHandle>(memberContext.memberHandle);
                    assert(entity && "Unexpected missing entity");

                    const auto* renderPrototype = entity->template get<RenderPrototypeComponent<TMemberHandle, Instanced>>();
                    assert(renderPrototype && "Unexpected missing RenderPrototypeComponent");

                    cmdBuffer.template add<RenderSceneMemberCommand<TMemberHandle>>(SceneMemberRenderContext<TMemberHandle>{
                        memberContext.memberHandle,
                        renderTargetHandle,
                        viewportHandle,
                        sceneHandle,
                        renderPrototype->meshHandle(),
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
        void dispatchInstancedRenderCommands(
            UpdateContext& updateContext,
            std::span<const std::vector<SceneMemberVisibilityContext<TMemberHandle, Instanced>>> visibilityContexts,
            TCommandBuffer& cmdBuffer) {

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
                        renderPrototype->shaderHandle() != renderBatchContext->shaderHandle) {

                            flushCurrentBatch();

                            renderBatchContext.emplace(
                                memberContext.renderTargetHandle,
                                viewportHandle,
                                sceneHandle,
                                renderPrototype->meshHandle(),
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
        using EngineRoleTag = helios::engine::runtime::world::tags::TypedSystemRole;

        /**
         * @brief Command buffer type used by this extraction system.
         */
        using CommandBuffer_type = TCommandBuffer;

        /**
         * @brief Constructs the system with the visibility snapshot registry.
         *
         * @param visibilityRegistry Registry containing per-frame visible/culled members.
         */
        explicit SceneRenderSystem(SceneMemberVisibilityRegistry<TMemberHandle>& visibilityRegistry)
        : visibilityRegistry_(visibilityRegistry) {
        }

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
        void update(UpdateContext& updateContext, TCommandBuffer& cmdBuffer) noexcept {

            auto sceneRenderContexts = visibilityRegistry_.sceneRenderContexts();

            for (auto& sceneRenderContext : sceneRenderContexts) {
                cmdBuffer.template add<RenderSceneCommand<TMemberHandle>>(sceneRenderContext);
            }

            const auto instancedMembers = visibilityRegistry_.template visibleMembers<Instanced>();
            dispatchInstancedRenderCommands(updateContext, instancedMembers, cmdBuffer);

            const auto nonInstancedMembers = visibilityRegistry_.template visibleMembers<NonInstanced>();
            dispatchNonInstancedRenderCommands(updateContext, nonInstancedMembers, cmdBuffer);


        }

    };

}