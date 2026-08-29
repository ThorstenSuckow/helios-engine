/**
 * @file RenderManager.ixx
 * @brief Batches render commands by target/viewport/material state and flushes them to the backend.
 */
module;

#include <vector>
#include <format>
#include "helios-engine-config.h"
#include <cassert>
#include <algorithm>
#include <iterator>

export module helios.engine.rendering.RenderManager;

import helios.engine.rendering.RenderDataResolver;
import helios.engine.rendering.RenderBackend;

import helios.engine.scene.components;

import helios.engine.rendering.common.commands;
import helios.engine.rendering.common.types;
import helios.engine.scene.types;
import helios.ecs;

import helios.core.log;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.core.container;

import helios.math;

import helios.engine.rendering.mesh.types;
import helios.engine.rendering.shader.types;

using namespace helios::engine::scene::components;
using namespace helios::engine::core::container;
using namespace helios::engine::runtime::world;
using namespace helios::ecs;
using namespace helios::engine::rendering::common::commands;
using namespace helios::engine::rendering::common::types;
using namespace helios::engine::rendering::mesh::types;
using namespace helios::engine::rendering::shader::types;
using namespace helios::engine::scene::types;
using namespace helios::ecs::common::types;
using namespace helios::engine::scene::types;
using namespace helios::core::log;
#define HELIOS_LOG_SCOPE "helios::engine::rendering::RenderManager"
export namespace helios::engine::rendering {

    /**
     * @brief Collects render commands into hierarchical batches and forwards them to the backend.
     *
     * @tparam TMemberHandle Renderable scene member handle type.
     */
    template<typename TRenderHandles, typename ...TMemberHandles>
    class RenderManager {

        using RenderTargetHandle = TRenderHandles::RenderTargetHandle;
        using ViewportHandle = TRenderHandles::ViewportHandle;
        using SceneHandle = TRenderHandles::SceneHandle;
        using TextureHandle = TRenderHandles::TextureHandle;
        using MaterialHandle = TRenderHandles::MaterialHandle;
        using ShaderHandle = TRenderHandles::ShaderHandle;
        using MeshHandle = TRenderHandles::MeshHandle;


        /**
         * @brief Converts an entity handle to a batch index.
         *
         * @tparam THandle Handle type.
         * @param handle Handle to convert.
         * @return Batch index corresponding to the handle.
         */
        template<typename THandle>
        [[nodiscard]] constexpr static std::size_t entityHandleToBatchIndex(const THandle handle) noexcept {
            return handle.isValid() ? handle.entityId() + 1 : 0;
        }

        /**
         * @brief Clears active child batches and resets the parent batch active flag.
         *
         * @tparam TBatch Parent batch type.
         * @tparam TChildHandle Child batch element type.
         * @param batch Parent batch to reset.
         * @param activeBatchIndices Active child indices to clear.
         * @param batches Child batch storage.
         */
        template<typename TBatch, typename TChildHandle>
        static void clearActive(TBatch* batch, std::vector<std::size_t>& activeBatchIndices, std::vector<TChildHandle>& batches) {
            batch->isActive = false;
            for (auto idx: activeBatchIndices) {
                batches[idx].clear();
            }
            activeBatchIndices.clear();
        }

        /**
         * @brief Returns the existing child batch for a handle or activates a new one.
         *
         * @details Creates a new batch with the specified handle as its owner, whereas invalid handles will all
         * be added to bucket with index = 0.
         * The index for buckets with valid handles will be incremented by 1 to avoid collision with the invalid handle bucket,
         * since (0, 1) is a valid Handle that would otherwise end up in bucket 0.
         *
         * @tparam THandle Handle type used as index source.
         * @tparam TChildBatch Child batch type.
         * @param handle Handle selecting the batch slot.
         * @param batches Child batch storage.
         * @param activeBatchIndices Active child indices list.
         * @return Reference to the active child batch.
         */
        template<typename THandle, typename TChildBatch>
        static TChildBatch& addToBatch(const THandle handle, std::vector<TChildBatch>& batches, std::vector<std::size_t>& activeBatchIndices) {

            auto idx = entityHandleToBatchIndex(handle);
            if (idx >= batches.size()) {
                batches.resize(idx + 1);
            }
            auto& batch = batches[idx];

            if (!batches[idx].isActive) {
                batch.handle = handle;
                batches[idx].isActive = true;
                activeBatchIndices.push_back(idx);
            }

            return batch;
        }


        /**
         * @brief Lowest-level batch collecting draw contexts for one mesh.
         */
        struct MeshBatch {
            bool isActive{false};
            MeshHandle handle;
            std::vector<DrawData> drawData;
            std::vector<InstanceData> instanceData;
            MeshBatch() {
                drawData.reserve(GAMEOBJECT_INITIAL_STORAGE_CAPACITY);
                instanceData.reserve(DEFAULT_INSTANCE_DATA_CAPACITY);
            }
            void clear() {
                isActive = false;
                drawData.clear();
                instanceData.clear();
            }
        };

        /**
         * @brief Groups mesh batches for one material.
         */
        struct MaterialBatch {
            bool isActive{false};
            MaterialHandle handle;
            std::vector<MeshBatch> batches;
            std::vector<std::size_t> activeBatchIndices;
            MaterialBatch(){batches.reserve(MESH_INITIAL_STORAGE_CAPACITY);}
            [[nodiscard]] MeshBatch& getOrAdd(MeshHandle handle) {
                return addToBatch(handle, batches, activeBatchIndices);
            }
            void clear() {
                clearActive(this, activeBatchIndices, batches);
            }
        };

        /**
         * @brief Groups material batches for one texture.
         */
        struct TextureBatch {
            bool isActive{false};
            TextureHandle handle;
            std::vector<MaterialBatch> batches;
            std::vector<std::size_t> activeBatchIndices;
            TextureBatch(){batches.reserve(MATERIAL_INITIAL_STORAGE_CAPACITY);}
            [[nodiscard]] MaterialBatch& getOrAdd(MaterialHandle handle) {
                return addToBatch(handle, batches, activeBatchIndices);
            }
            void clear() {
                clearActive(this, activeBatchIndices, batches);
            }
        };

        /**
         * @brief Groups texture batches for one shader.
         */
        struct ShaderBatch {
            bool isActive{false};
            ShaderHandle handle;
            std::vector<TextureBatch> batches;
            std::vector<std::size_t> activeBatchIndices;
            ShaderBatch(){batches.reserve(TEXTURE_INITIAL_STORAGE_CAPACITY);}
            [[nodiscard]] TextureBatch& getOrAdd(TextureHandle handle) {
                return addToBatch(handle, batches, activeBatchIndices);
            }
            void clear() {
                clearActive(this, activeBatchIndices, batches);
            }
        };

        /**
         * @brief Groups shader batches for one viewport.
         */
        struct ViewportBatch {
            bool isActive{false};
            ViewportHandle handle;
            std::vector<ShaderBatch> batches;
            std::vector<std::size_t> activeBatchIndices;
            [[nodiscard]] ShaderBatch& getOrAdd(ShaderHandle handle) {
                return addToBatch(handle, batches, activeBatchIndices);
            }
            ViewportBatch(){batches.reserve(SHADER_INITIAL_STORAGE_CAPACITY);}
            void clear() {
                clearActive(this, activeBatchIndices, batches);
            }
        };

        /**
         * @brief Top-level batch grouping viewport batches per render target.
         */
        struct RenderTargetBatch {
            bool isActive{false};
            RenderTargetHandle handle;
            std::vector<ViewportBatch> batches;
            std::vector<std::size_t> activeBatchIndices;
            [[nodiscard]] ViewportBatch& getOrAdd(ViewportHandle handle) {
                return addToBatch(handle, batches, activeBatchIndices);
            }
            RenderTargetBatch(){batches.reserve(VIEWPORT_INITIAL_STORAGE_CAPACITY);}
            void clear() {
                clearActive(this, activeBatchIndices, batches);
            }
        };


        /**
         * @brief Scoped logger used for debug and diagnostics output.
         */
        inline static auto& logger_ = LogManager::loggerForScope(HELIOS_LOG_SCOPE);


        /**
         * @brief Top-level render-target batch storage indexed by handle entity id.
         */
        std::vector<RenderTargetBatch> renderTargetBatches_;

        /**
         * @brief Active render-target indices used for frame-local flush traversal.
         */
        std::vector<EntityId> activeRenderTargetIndices_;


        /**
         * @brief Ensures that the render-target and viewport batch nodes exist and are active.
         *
         * @param renderTargetHandle Render target used as top-level batch key.
         * @param viewportHandle Viewport used as second-level batch key.
         * @return Active viewport batch for the given handles.
         */
        [[nodiscard]] ViewportBatch& ensureViewportBatch(
            RenderTargetHandle renderTargetHandle, ViewportHandle viewportHandle) {

            auto renderTargetId = renderTargetHandle.entityId();

            if (renderTargetBatches_.size() <= renderTargetId) {
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
         * @details Batches are scoped by render target and viewport.
         * A viewport is bound to exactly one scene, so sceneHandle is not required
         * as part of the batch key.
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
            auto& textureBatch = shaderBatch.getOrAdd(renderContext.textureHandle);
            auto& materialBatch = textureBatch.getOrAdd(renderContext.materialHandle);
            auto& meshBatch = materialBatch.getOrAdd(renderContext.meshHandle);

            return meshBatch;
        }

    public:



        /**
         * @brief Flushes all active render-target batches to the backend.
         */
        bool executeCommands(ecs::common::container::EcsDataContainer& ecsDataContainer,
            RenderDataResolver& renderDataResolver, RenderBackend& renderBackend) noexcept {


            for (auto renderTargetIdx : activeRenderTargetIndices_) {
                auto& renderTargetBatch = renderTargetBatches_[renderTargetIdx];

                if (auto renderData = renderDataResolver.resolveRenderTargetData(ecsDataContainer, renderTargetBatch.handle.value())) {
                    renderBackend.beginRenderTargetBatch(*renderData);
                } else {
                    logger_.error("Failed to resolve render target data for batch");
                    assert(false && "Failed to resolve render target data for batch");
                    continue;
                }

                for (auto viewportIdx : renderTargetBatch.activeBatchIndices) {
                    auto& viewportBatch = renderTargetBatch.batches[viewportIdx];

                    if (auto renderData = renderDataResolver.resolveViewportData(ecsDataContainer, viewportBatch.handle.value())) {
                        renderBackend.beginViewportBatch(*renderData);
                    } else {
                        logger_.error("Failed to resolve viewport data for batch");
                        assert(false && "Failed to resolve viewport data for batch");
                        continue;
                    }

                    for (auto shaderIdx : viewportBatch.activeBatchIndices ) {
                        auto& shaderBatch = viewportBatch.batches[shaderIdx];

                        if (auto renderData = renderDataResolver.resolveShaderData(ecsDataContainer, shaderBatch.handle.value())) {
                            renderBackend.beginShaderBatch(*renderData);
                        } else {
                            logger_.error("Failed to resolve shader data for batch");
                            assert(false && "Failed to resolve shader data for batch");
                            continue;
                        }

                        for (auto textureIdx : shaderBatch.activeBatchIndices ) {
                            auto& textureBatch = shaderBatch.batches[textureIdx];

                            auto validTextureBatch = textureBatch.handle.isValid();
                            // consider invalid (intentionally missing) textures
                            if (validTextureBatch) {

                                if (auto renderData = renderDataResolver.resolveTextureData(ecsDataContainer, textureBatch.handle.value())) {
                                    renderBackend.beginTextureBatch(*renderData);
                                } else {
                                    logger_.error("Failed to resolve texture data for batch");
                                    assert(false && "Failed to resolve texture data for batch");
                                    continue;
                                }

                            }

                            for (auto materialIdx : textureBatch.activeBatchIndices) {
                                auto& materialBatch = textureBatch.batches[materialIdx];

                                if (auto renderData = renderDataResolver.resolveMaterialData(ecsDataContainer, materialBatch.handle.value())) {
                                    renderBackend.beginMaterialBatch(*renderData);
                                } else {
                                    logger_.error("Failed to resolve material data for batch");
                                    assert(false && "Failed to resolve material data for batch");
                                    continue;
                                }

                                for (auto meshIdx : materialBatch.activeBatchIndices) {
                                    auto& meshBatch = materialBatch.batches[meshIdx];

                                    if (auto renderData = renderDataResolver.resolveMeshData(ecsDataContainer, meshBatch.handle.value())) {
                                        renderBackend.beginMeshBatch(*renderData);
                                    } else {
                                        logger_.error("Failed to resolve mesh data for batch");
                                        assert(false && "Failed to resolve mesh data for batch");
                                        continue;
                                    }


                                    renderBackend.renderBatch(meshBatch.drawData);
                                    renderBackend.renderBatch(meshBatch.instanceData);

                                    renderBackend.endMeshBatch();
                                } // materialBatch

                                renderBackend.endMaterialBatch();
                            } //textureBatch

                            if (validTextureBatch) {
                                renderBackend.endTextureBatch();
                            }
                        } // shader batch

                        renderBackend.endShaderBatch();
                    } // viewportBatch

                    renderBackend.endViewportBatch();
                } //renderTargetBatch

                renderBackend.endRenderTargetBatch();
            }

            for (auto idx : activeRenderTargetIndices_ ) {
                renderTargetBatches_[idx].clear();
            }
            activeRenderTargetIndices_.clear();

            return true;
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
        template<typename TMemberHandle>
        bool submit(RenderSceneCommand<TMemberHandle, TRenderHandles>&& renderSceneCommand) noexcept {

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
        template<typename TMemberHandle>
        bool submit(RenderSceneMemberCommand<TMemberHandle, TRenderHandles>&& renderCommand) noexcept {

            auto renderContext = std::move(renderCommand.sceneMemberRenderContext);

            auto& meshBatch = meshBatchFor(renderContext);

            meshBatch.drawData.push_back({
                renderContext.worldMatrix
            });

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
        template<typename TMemberHandle>
        bool submit(RenderInstanceBatchCommand<TMemberHandle, TRenderHandles>&& renderCommand) noexcept {

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
        bool init(ecs::command::CommandHandlerRegistry& commandHandlerRegistry) noexcept {

            (commandHandlerRegistry.template handleCommands<
                RenderSceneMemberCommand<TMemberHandles, TRenderHandles>,
                RenderInstanceBatchCommand<TMemberHandles, TRenderHandles>,
                RenderSceneCommand<TMemberHandles, TRenderHandles>
            >(*this), ...);

            return true;

        };


        void reset(){/* intentionally noop */}


    };


}