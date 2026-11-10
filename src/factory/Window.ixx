module;

#include <cstdint>
#include <string>

export module helios.engine.factory:Window;

import helios.ecs.entity;
import helios.ecs.entity.EntityWorld;
import helios.engine.platform.window.components.WindowCreateRequestComponent;
import helios.engine.platform.window.types.WindowConfig;
import helios.engine.rendering.common.components;
import helios.math;

import :RenderTarget;
import :Viewport;
import :ViewportFactory;

export namespace helios::engine::factory {


    template<typename TWindowHandle, typename TRenderHandles>
    class Window {

        using WindowCreateRequestComponent = helios::engine::platform::window::components::WindowCreateRequestComponent;
        using WindowConfig = helios::engine::platform::window::types::WindowConfig;
        using EntityManager = ecs::entity::EntityManager<TWindowHandle>;
        using WindowEntity = ecs::entity::Entity<EntityManager>;

        using RenderTargetEntity = ecs::entity::Entity<ecs::entity::EntityManager<typename TRenderHandles::RenderTargetHandle>>;

        using RenderTargetBindingComponent = helios::engine::rendering::common::components::RenderTargetBindingComponent<TRenderHandles>;

        using WindowRenderTarget = RenderTarget<TRenderHandles>;
        using WindowViewport = Viewport<TRenderHandles>;

        // lives as long as the EntityWorld; factories are created on demand
        ecs::entity::EntityWorld& ecsWorld_;

        WindowEntity windowEntity_;

        RenderTargetEntity renderTargetEntity_;

    public:

        explicit Window(
            ecs::entity::EntityWorld& ecsWorld,
            WindowEntity windowEntity,
            const std::string& title,
            const std::uint32_t width,
            const std::uint32_t height,
            RenderTargetEntity renderTargetEntity
        ) : ecsWorld_(ecsWorld),
            windowEntity_(windowEntity),
            renderTargetEntity_(renderTargetEntity) {

            windowEntity_.template add<
                WindowCreateRequestComponent
            >(WindowConfig{
                .title = title,
                .size = {static_cast<float>(width), static_cast<float>(height)},
                .aspectRatioNumer = 0,
                .aspectRatioDenom = 0,
                .vsyncEnabled = false
            });

            windowEntity_.template add<RenderTargetBindingComponent>(renderTargetEntity_.handle());
        }

        Window& enforceAspectRatio(const float num, const float den) noexcept {
            auto& windowConfig = windowEntity_.track(
                windowEntity_.template get<WindowCreateRequestComponent>())->windowConfig;
            windowConfig.aspectRatioNumer = num;
            windowConfig.aspectRatioDenom = den;
            return *this;
        }

        Window& enableVSync(const bool enabled) noexcept {
            auto& windowConfig = windowEntity_.track(
                windowEntity_.template get<WindowCreateRequestComponent>())->windowConfig;
            windowConfig.vsyncEnabled = enabled;
            return *this;
        }

        /**
         * @brief Creates a viewport covering `rect` of this window's render target.
         *
         * @details Creation is delegated to ViewportFactory, which keeps the
         * invariants (active entity, render target binding) in one place.
         */
        WindowViewport addViewport(const helios::math::vec4f rect) {
            auto viewport = ViewportFactory(ecsWorld_).createViewport<TRenderHandles>(
                rect, renderTargetEntity_);
            return viewport;
        }

        WindowRenderTarget renderTarget() const noexcept {
            return WindowRenderTarget(renderTargetEntity_);
        }

        WindowEntity entity() const noexcept {
            return windowEntity_;
        }

        TWindowHandle handle() const noexcept {
            return windowEntity_.handle();
        }

    };

}