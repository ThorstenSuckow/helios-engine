/**
 * @file RenderManager.ixx
 * @brief Batches render commands by target/viewport/material state and flushes them to the backend.
 */
module;

#include <vector>
#include <format>
#include "helios-engine-config.h"
#include <optional>
#include <algorithm>
#include <iterator>

export module helios.engine.rendering.RenderManager;

import helios.engine.runtime.world.tags.ManagerRole;

import helios.engine.rendering.renderTarget.types.RenderTargetHandle;
import helios.engine.rendering.viewport.types.ViewportHandle;
import helios.engine.scene.types.SceneHandle;

import helios.engine.scene.components;

import helios.engine.rendering.common.commands;
import helios.engine.rendering.common.types;
import helios.engine.scene.types;
import helios.engine.runtime.messaging.command.CommandHandlerRegistry;

import helios.ecs.types;

import helios.engine.util.log;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.rendering.common.concepts.IsRenderBackendLike;

import helios.engine.core.container.HandleMultiMap;

import helios.math;

import helios.engine.rendering.mesh.types;
import helios.engine.rendering.material.types;
import helios.engine.rendering.shader.types;

using namespace helios::engine::scene::components;
using namespace helios::engine::core::container;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::world::tags;
using namespace helios::engine::runtime::messaging::command;
using namespace helios::engine::rendering::common::commands;
using namespace helios::engine::rendering::common::types;
using namespace helios::engine::rendering::mesh::types;
using namespace helios::engine::rendering::material::types;
using namespace helios::engine::rendering::shader::types;
using namespace helios::engine::rendering::viewport::types;

using namespace helios::engine::rendering::renderTarget::types;
using namespace helios::engine::rendering::viewport::types;
using namespace helios::engine::scene::types;
using namespace helios::ecs::types;
using namespace helios::engine::scene::types;
using namespace helios::engine::util::log;
using namespace helios::engine::rendering::common::concepts;

#define HELIOS_LOG_SCOPE "helios::engine::rendering::RenderManager"
export namespace helios::engine::rendering {

    /**
     * @brief Collects render commands into hierarchical batches and forwards them to the backend.
     *
     * @details
     * Commands are grouped by render-target, viewport, shader, material, and mesh.
     * `flush(...)` iterates this hierarchy and calls the corresponding backend batch hooks.
     *
     * @tparam TRenderBackend Rendering backend type.
     * @tparam TMemberHandle Renderable scene member handle type.
     */
    template<typename TRenderBackend, typename TMemberHandle>
    requires IsRenderBackendLike<TRenderBackend, TMemberHandle>
    class RenderManager {

        /**
         * @brief Clears active child batches and resets the parent batch active flag.
         *
         * @tparam TBatch Parent batch type.
         * @tparam TChildHandle Child batch element type.
         * @param batch Parent batch to reset.
         * @param activeIndices Active child indices to clear.
         * @param batches Child batch storage.
         */
        template<typename TBatch, typename TChildHandle>
        static void clearActive(TBatch* batch, std::vector<EntityId>& activeIndices, std::vector<TChildHandle>& batches) {
            batch->isActive = false;
            for (auto idx: activeIndices) {
                batches[idx].clear();
            }
            activeIndices.clear();
        }

        /**
         * @brief Returns the existing child batch for a handle or activates a new one.
         *
         * @tparam THandle Handle type used as index source.
         * @tparam TChildBatch Child batch type.
         * @param handle Handle selecting the batch slot.
         * @param batches Child batch storage.
         * @param activeIndices Active child indices list.
         * @return Reference to the active child batch.
         */
        template<typename THandle, typename TChildBatch>
        static TChildBatch& addToBatch(const THandle handle, std::vector<TChildBatch>& batches, std::vector<EntityId>& activeIndices) {
            if (handle.entityId >= batches.size()) {
                batches.resize(handle.entityId + 1);
            }
            auto& batch = batches[handle.entityId];

            if (!batches[handle.entityId].isActive) {
                batch.handle = handle;
                batches[handle.entityId].isActive = true;
                activeIndices.push_back(handle.entityId);
            }

            return batch;
        }

        /**
         * @brief Lowest-level batch collecting draw contexts for one mesh.
         *
         * @tparam TDrawMemberHandle Draw/member handle type.
         */
        template<typename TDrawMemberHandle>
        struct MeshBatch {
            bool isActive{false};
            MeshHandle handle;
            std::vector<SceneMemberRenderContext<TDrawMemberHandle>> drawContexts;
            std::vector<InstanceData> instanceData;
            MeshBatch() {
                drawContexts.reserve(DEFAULT_GAMEOBJECT_CAPACITY);
                instanceData.reserve(DEFAULT_INSTANCE_DATA_CAPACITY);
            }
            void clear() {
                isActive = false;
                drawContexts.clear();
                instanceData.clear();
            }
        };

        /**
         * @brief Groups mesh batches for one material.
         *
         * @tparam TDrawMemberHandle Draw/member handle type.
         */
        template<typename TDrawMemberHandle>
        struct MaterialBatch {
            bool isActive{false};
            MaterialHandle handle;
            std::vector<MeshBatch<TDrawMemberHandle>> batches;
            std::vector<EntityId> activeIndices;
            MaterialBatch(){batches.reserve(DEFAULT_MESH_POOL_CAPACITY);}
            [[nodiscard]] MeshBatch<TDrawMemberHandle>& getOrAdd(MeshHandle handle) {
                return addToBatch(handle, batches, activeIndices);
            }
            void clear() {
                clearActive(this, activeIndices, batches);
            }
        };

        /**
         * @brief Groups material batches for one shader.
         *
         * @tparam TDrawMemberHandle Draw/member handle type.
         */
        template<typename TDrawMemberHandle>
        struct ShaderBatch {
            bool isActive{false};
            ShaderHandle handle;
            std::vector<MaterialBatch<TDrawMemberHandle>> batches;
            std::vector<EntityId> activeIndices;
            ShaderBatch(){batches.reserve(DEFAULT_MATERIAL_POOL_CAPACITY);}
            [[nodiscard]] MaterialBatch<TDrawMemberHandle>& getOrAdd(MaterialHandle handle) {
                return addToBatch(handle, batches, activeIndices);
            }
            void clear() {
                clearActive(this, activeIndices, batches);
            }
        };

        /**
         * @brief Groups shader batches for one viewport.
         *
         * @tparam TDrawMemberHandle Draw/member handle type.
         */
        template<typename TDrawMemberHandle>
        struct ViewportBatch {
            bool isActive{false};
            ViewportHandle handle;
            std::vector<ShaderBatch<TDrawMemberHandle>> batches;
            std::vector<EntityId> activeIndices;
            [[nodiscard]] ShaderBatch<TDrawMemberHandle>& getOrAdd(ShaderHandle handle) {
                return addToBatch(handle, batches, activeIndices);
            }
            ViewportBatch(){batches.reserve(DEFAULT_SHADER_POOL_CAPACITY);}
            void clear() {
                clearActive(this, activeIndices, batches);
            }
        };

        /**
         * @brief Top-level batch grouping viewport batches per render target.
         *
         * @tparam TDrawMemberHandle Draw/member handle type.
         */
        template<typename TDrawMemberHandle>
        struct RenderTargetBatch {
            bool isActive{false};
            RenderTargetHandle handle;
            std::vector<ViewportBatch<TDrawMemberHandle>> batches;
            std::vector<EntityId> activeIndices;
            [[nodiscard]] ViewportBatch<TDrawMemberHandle>& getOrAdd(ViewportHandle handle) {
                return addToBatch(handle, batches, activeIndices);
            }
            RenderTargetBatch(){batches.reserve(DEFAULT_VIEWPORT_POOL_CAPACITY);}
            void clear() {
                clearActive(this, activeIndices, batches);
            }
        };


        /**
         * @brief Scoped logger used for debug and diagnostics output.
         */
        inline static auto& logger_ = LogManager::loggerForScope(HELIOS_LOG_SCOPE);


        /**
         * @brief Top-level render-target batch storage indexed by handle entity id.
         */
        std::vector<RenderTargetBatch<TMemberHandle>> renderTargetBatches_;

        /**
         * @brief Active render-target indices used for frame-local flush traversal.
         */
        std::vector<EntityId> activeRenderTargetIndices_;

        /**
         * @brief Backend instance that receives begin/end hooks and draw submissions.
         */
        TRenderBackend& renderBackend_;

        /**
         * @brief Ensures that the render-target and viewport batch nodes exist and are active.
         *
         * @param renderTargetHandle Render target used as top-level batch key.
         * @param viewportHandle Viewport used as second-level batch key.
         * @return Active viewport batch for the given handles.
         */
        [[nodiscard]] ViewportBatch<TMemberHandle>& ensureViewportBatch(
            RenderTargetHandle renderTargetHandle, ViewportHandle viewportHandle) {

            auto renderTargetId = renderTargetHandle.entityId;

            if (renderTargetBatches_.size() <= renderTargetHandle.entityId) {
                renderTargetBatches_.resize(renderTargetId + 1);
            }

            auto& renderTargetBatch = renderTargetBatches_[renderTargetId];

            if (!renderTargetBatch.isActive) {
                renderTargetBatch.isActive = true;
                renderTargetBatch.handle = renderTargetHandle;
                activeRenderTargetIndices_.push_back(renderTargetId);
            }

            return renderTargetBatch.getOrAdd(viewportHandle);
        };

        /**
         * @brief Resolves the mesh batch for a render context and activates missing hierarchy nodes.
         *
         * @tparam TContextType Context type exposing render target, viewport, shader,
         * material, and mesh handles.
         * @param renderContext Render context used as batch key source.
         * @return Reference to the resolved mesh batch.
         */
        template<typename TContextType>
        auto& meshBatchFor(TContextType& renderContext) noexcept {

            auto& viewportBatch = ensureViewportBatch(renderContext.renderTargetHandle, renderContext.viewportHandle);
            auto& shaderBatch = viewportBatch.getOrAdd(renderContext.shaderHandle);
            auto& materialBatch = shaderBatch.getOrAdd(renderContext.materialHandle);
            auto& meshBatch = materialBatch.getOrAdd(renderContext.meshHandle);

            return meshBatch;
        }

    public:

        /**
         * @brief Runtime role tag used for engine manager registration.
         */
        using EngineRoleTag = ManagerRole;

        /**
         * @brief Constructs the manager for a specific render backend.
         *
         * @param renderBackend Backend used to execute render passes.
         */
        explicit RenderManager(TRenderBackend& renderBackend) : renderBackend_(renderBackend) {

            renderTargetBatches_.reserve(DEFAULT_FRAMEBUFFER_POOL_CAPACITY);

        }


        /**
         * @brief Flushes all active render-target batches to the backend.
         *
         * @details Traverses active render targets and nested viewport/shader/material/mesh batches,
         * executes backend begin/end hooks for each level, renders queued draw contexts,
         * and clears all active batch indices afterwards.
         *
         * @param updateContext Current frame update context.
         */
        void flush(UpdateContext& updateContext) {

            for (auto renderTargetIdx : activeRenderTargetIndices_) {
                auto& renderTargetBatch = renderTargetBatches_[renderTargetIdx];

                renderBackend_.beginRenderTargetBatch(renderTargetBatch.handle);

                for (auto viewportIdx : renderTargetBatch.activeIndices) {
                    auto& viewportBatch = renderTargetBatch.batches[viewportIdx];

                    renderBackend_.beginViewportBatch(viewportBatch.handle);

                    for (auto shaderIdx : viewportBatch.activeIndices ) {
                        auto& shaderBatch = viewportBatch.batches[shaderIdx];

                        renderBackend_.beginShaderBatch(shaderBatch.handle);

                        for (auto materialIdx : shaderBatch.activeIndices) {
                            auto& materialBatch = shaderBatch.batches[materialIdx];

                            renderBackend_.beginMaterialBatch(materialBatch.handle);

                            for (auto meshIdx : materialBatch.activeIndices) {
                                auto& meshBatch = materialBatch.batches[meshIdx];

                                renderBackend_.beginMeshBatch(meshBatch.handle);

                                renderBackend_.template renderBatch<TMemberHandle>(meshBatch.drawContexts, meshBatch.instanceData);

                                renderBackend_.endMeshBatch(meshBatch.handle);
                            } // materialBatch

                            renderBackend_.endMaterialBatch(materialBatch.handle);
                        } //shaderBatch

                        renderBackend_.endShaderBatch(shaderBatch.handle);
                    } // viewportBatch

                    renderBackend_.endViewportBatch(viewportBatch.handle);
                } //renderTargetBatch

                renderBackend_.endRenderTargetBatch(renderTargetBatch.handle);
            }

            for (auto idx : activeRenderTargetIndices_ ) {
                renderTargetBatches_[idx].clear();
            }
            activeRenderTargetIndices_.clear();


        }


        /**
         * @brief Queues one scene render context into the batch structure.
         *
         * @details Ensure that the ViewportBatch is available for the next flush().
         * In case all scene members were culled, i.e. the shader/material/mesh batches
         * are empty, viewport batches are sent to the graphics backend for making sure no
         * artifacts are left in the viewport from the previous render operation, if any.
         *
         * @param renderSceneCommand Command containing scene-level render context.
         * @return `true` if the command was accepted.
         */
        bool submit(RenderSceneCommand<TMemberHandle>&& renderSceneCommand) noexcept {

            std::ignore = ensureViewportBatch(
                renderSceneCommand.sceneRenderContext.renderTargetHandle,
                renderSceneCommand.sceneRenderContext.viewportHandle
            );

            return true;
        }


        /**
         * @brief Queues one draw context into the hierarchical batch structure.
         *
         * @details Activates missing nodes for render target, viewport, shader, material, and mesh,
         * then appends the draw context to the mesh batch for later rendering in `flush(...)`.
         *
         * @param renderCommand Command containing per-member render context.
         * @return `true` if the command was accepted.
         */
        bool submit(RenderSceneMemberCommand<TMemberHandle>&& renderCommand) noexcept {

            auto renderContext = std::move(renderCommand.sceneMemberRenderContext);

            auto& meshBatch = meshBatchFor(renderContext);

            meshBatch.drawContexts.push_back(std::move(renderContext));

            return true;
        }

        /**
         * @brief Queues an instanced draw batch into the hierarchical batch structure.
         *
         * @details Activates missing nodes for render target, viewport, shader, material, and mesh,
         * then appends or moves instance payload into the mesh batch for later rendering in `flush(...)`.
         *
         * @param renderCommand Command containing instance batch context.
         * @return `true` if the command was accepted.
         */
        bool submit(RenderInstanceBatchCommand<TMemberHandle>&&renderCommand) noexcept {

            auto renderContext = std::move(renderCommand.instanceRenderBatchContext);

            auto& meshBatch = meshBatchFor(renderContext);

            if (meshBatch.instanceData.empty()) {
                meshBatch.instanceData = std::move(renderContext.instanceData);
            } else {
                meshBatch.instanceData.reserve(meshBatch.instanceData.size() + renderContext.instanceData.size());
                std::ranges::move(renderContext.instanceData, std::back_inserter(meshBatch.instanceData));
            }

            return true;
        }


        /**
         * @brief Registers this manager as handler for render commands.
         *
         * @param commandHandlerRegistry Command handler registry used at runtime.
         */
        void init(CommandHandlerRegistry& commandHandlerRegistry) noexcept {

            commandHandlerRegistry.handleCommands<
                RenderSceneMemberCommand<TMemberHandle>,
                RenderInstanceBatchCommand<TMemberHandle>,
                RenderSceneCommand<TMemberHandle>
            >(*this);


        };


    };


}