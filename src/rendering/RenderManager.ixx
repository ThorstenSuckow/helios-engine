/**
 * @file RenderManager.ixx
 * @brief Draft render manager that batches render commands per scene and target.
 */
module;

#include <vector>
#include <format>
#include "helios-engine-config.h"
#include <optional>

export module helios.engine.rendering.RenderManager;

import helios.engine.runtime.world.tags.ManagerRole;

import helios.engine.rendering.framebuffer.types.FramebufferHandle;
import helios.engine.rendering.viewport.types.ViewportHandle;
import helios.engine.scene.types.SceneHandle;

import helios.engine.scene.components;

import helios.engine.rendering.common.commands.RenderCommand;
import helios.engine.rendering.common.types.RenderPassContext;
import helios.engine.scene.types.SceneMemberRenderContext;
import helios.engine.runtime.messaging.command.CommandHandlerRegistry;

import helios.ecs.types.TypeDefs;

import helios.engine.util.log;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.rendering.common.concepts.IsRenderBackendLike;

import helios.engine.core.container.HandleMultiMap;

import helios.math;

using namespace helios::engine::scene::components;
using namespace helios::engine::core::container;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::world::tags;
using namespace helios::engine::runtime::messaging::command;
using namespace helios::engine::rendering::common::commands;
using namespace helios::engine::rendering::common::types;

using namespace helios::engine::rendering::framebuffer::types;
using namespace helios::engine::rendering::viewport::types;
using namespace helios::engine::scene::types;

using namespace helios::engine::scene::types;
using namespace helios::engine::util::log;
using namespace helios::engine::rendering::common::concepts;

#define HELIOS_LOG_SCOPE "helios::engine::rendering::RenderManager"
export namespace helios::engine::rendering {


    /**
     * @brief Draft manager that collects render commands and drives render passes.
     *
     * @tparam TRenderBackend Rendering backend type.
     * @tparam TMemberHandle Renderable scene member handle type.
     */
    template<typename TRenderBackend, typename TMemberHandle>
    requires IsRenderBackendLike<TRenderBackend, TMemberHandle>
    class RenderManager {

        /**
         * @brief Per-viewport camera matrices used for a render pass.
         */
        struct ViewProjection {
            helios::math::mat4f viewMatrix;
            helios::math::mat4f projectionMatrix;
        };

        inline static auto& logger_ = LogManager::loggerForScope(HELIOS_LOG_SCOPE);


        HandleMultiMap<FramebufferHandle, ViewportHandle> framebufferToViewport_;
        HandleMultiMap<SceneHandle, ViewportHandle> sceneToViewport_;
        std::vector<std::vector<SceneMemberRenderContext<TMemberHandle>>> sceneMemberRenderContexts_;

        TRenderBackend& renderBackend_;

        /**
         * @brief Resolves view and projection matrices for a viewport's bound camera.
         *
         * @param updateContext Current frame update context.
         * @param viewportHandle Viewport to resolve camera matrices for.
         * @return View/projection pair on success, otherwise `std::nullopt`.
         */
        [[nodiscard]] std::optional<ViewProjection> viewProjection(
            UpdateContext& updateContext, ViewportHandle viewportHandle
        ) const noexcept {
            auto viewport = updateContext.find(viewportHandle);

            if (!viewport) {
                logger_.error("Expected ViewportEntity, but couldn't find any.");
                return std::nullopt;
            }
            auto* cbc = viewport->get<CameraBindingComponent<ViewportHandle>>();
            if (!cbc) {
                logger_.error("Expected CameraBindingComponent on ViewportEntity, but couldn't find any.");
                return std::nullopt;
            }
            auto camera = updateContext.find(cbc->targetHandle());
            if (!camera) {
                logger_.error("Expected CameraEntity, but couldn't find any.");
                return std::nullopt;
            }
            using CameryHandleType = std::remove_cvref_t<decltype(cbc->targetHandle())>;
            auto* vm = camera->get<ViewMatrixComponent<CameryHandleType>>();
            if (!vm) {
                logger_.error("Expected ViewMatrixComponent, but couldn't find any.");
                return std::nullopt;
            }

            auto* pm = camera->get<ProjectionMatrixComponent<CameryHandleType>>();
            if (!pm) {
                logger_.error("Expected ProjectionMatrixComponent, but couldn't find any.");
                return std::nullopt;
            }

            return ViewProjection{
                vm->value(), pm->value()
            };

        }

    public:

        /** @brief Runtime role tag used for engine manager registration. */
        using EngineRoleTag = ManagerRole;

        /**
         * @brief Constructs the manager for a specific render backend.
         *
         * @param renderBackend Backend used to execute render passes.
         */
        explicit RenderManager(TRenderBackend& renderBackend) : renderBackend_(renderBackend) {

            framebufferToViewport_.reserve(DEFAULT_FRAMEBUFFER_POOL_CAPACITY, DEFAULT_VIEWPORT_POOL_CAPACITY);
            sceneToViewport_.reserve(DEFAULT_SCENE_POOL_CAPACITY, DEFAULT_VIEWPORT_POOL_CAPACITY);
            sceneMemberRenderContexts_.reserve(DEFAULT_SCENE_POOL_CAPACITY);

        }


        /**
         * @brief Flushes queued render work to the backend.
         *
         * @details Iterates framebuffer/viewport bindings, prepares one render-pass context
         * per pair, dispatches all queued member render contexts for the associated
         * scene, then clears all internal queues.
         *
         * @param updateContext Current frame update context.
         */
        void flush(UpdateContext& updateContext) {

            auto prevViewportHandle = ViewportHandle{};

            auto renderPassContext = RenderPassContext{};

            for (auto [framebufferHandle, viewportHandle]: framebufferToViewport_) {

                    renderPassContext.framebufferHandle = framebufferHandle;
                    renderPassContext.viewportHandle    = viewportHandle;

                    if (viewportHandle != prevViewportHandle) {

                        auto vp = viewProjection(updateContext, viewportHandle);
                        if (!vp) {
                            logger_.warn("Could not determine View/Projection-matrices for RenderPass");
                            renderPassContext.viewMatrix       = helios::math::mat4f{1.0f};
                            renderPassContext.projectionMatrix = helios::math::mat4f{1.0f};
                            continue;

                        }
                        renderPassContext.viewMatrix       = vp->viewMatrix;
                        renderPassContext.projectionMatrix = vp->projectionMatrix;

                        prevViewportHandle = viewportHandle;
                    }


                    renderBackend_.beginRenderPass(renderPassContext);

                    auto sceneHandle = sceneToViewport_.key(viewportHandle);

                    auto& sceneMemberRenderContexts = sceneMemberRenderContexts_[sceneHandle.entityId];


                    for (auto renderContext : sceneMemberRenderContexts) {
                        renderBackend_.doRender(renderContext);
                    }


                   renderBackend_.endRenderPass(renderPassContext);


            }

            framebufferToViewport_.clear();
            sceneToViewport_.clear();
            sceneMemberRenderContexts_.clear();
        }

        /**
         * @brief Queues a render command for later execution.
         *
         * @details Stores framebuffer/viewport and scene/viewport bindings and appends the
         * member render context to the scene bucket addressed by `sceneHandle`.
         *
         * @param renderCommand Command containing per-member render context.
         * @return `true` if the command was accepted.
         */
        bool submit(RenderCommand<TMemberHandle> renderCommand) noexcept {

            const auto& sceneMemberRenderContext = renderCommand.sceneMemberRenderContext;
            const auto sceneIdx = sceneMemberRenderContext.sceneHandle.entityId;

            framebufferToViewport_.bind(
                sceneMemberRenderContext.framebufferHandle,
                sceneMemberRenderContext.viewportHandle
            );

            sceneToViewport_.bind(
                sceneMemberRenderContext.sceneHandle,
                sceneMemberRenderContext.viewportHandle
            );

            if (sceneMemberRenderContexts_.size() <= sceneIdx) {
                sceneMemberRenderContexts_.resize(sceneIdx + 1);
            }

            sceneMemberRenderContexts_[sceneIdx].push_back(std::move(renderCommand.sceneMemberRenderContext));

            return true;
        }

        /**
         * @brief Registers this manager as a render command handler.
         *
         * @param commandHandlerRegistry Command handler registry used at runtime.
         */
        void init(CommandHandlerRegistry& commandHandlerRegistry) noexcept {

            commandHandlerRegistry.handleCommands<
                RenderCommand<TMemberHandle>
            >(*this);


        };


    };


}