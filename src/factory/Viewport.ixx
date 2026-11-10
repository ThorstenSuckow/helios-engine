module;

export module helios.engine.factory:Viewport;

import :Scene;
import :Camera;

import helios.ecs.entity;
import helios.engine.rendering.common.components;
import helios.engine.rendering.common.types;
import helios.engine.core.components;
import helios.engine.spatial.components;
import helios.math;

export namespace helios::engine::factory {

    class ViewportFactory;

    /**
     * @brief Configuration handle for a viewport entity.
     *
     * @details Instances can only be created by ViewportFactory, which guarantees
     * that the entity is active and bound to a render target.
     */
    template<typename TRenderHandles>
    class Viewport {

        friend class ViewportFactory;

        using ViewportHandle = typename TRenderHandles::ViewportHandle;
        using RenderTargetHandle = typename TRenderHandles::RenderTargetHandle;

        using EntityManager = ecs::entity::EntityManager<ViewportHandle>;
        using ViewportEntity = ecs::entity::Entity<EntityManager>;

        using RenderTargetEntity = ecs::entity::Entity<ecs::entity::EntityManager<RenderTargetHandle>>;

        using RenderTargetBindingComponent = helios::engine::rendering::common::components::RenderTargetBindingComponent<TRenderHandles>;

        using SceneBindingComponent = helios::engine::scene::components::SceneBindingComponent<TRenderHandles>;
        using CameraBindingComponent = helios::engine::scene::components::CameraBindingComponent<TRenderHandles>;

        using RectComponent = helios::engine::spatial::components::RectComponent;
        using ColorComponent = helios::engine::core::components::ColorComponent;
        using ClearComponent = helios::engine::rendering::common::components::ClearComponent;
        using ClearFlags = helios::engine::rendering::common::types::ClearFlags;

        ViewportEntity viewportEntity_;
        RenderTargetEntity renderTargetEntity_;

        explicit Viewport(
            ViewportEntity viewportEntity,
            RenderTargetEntity renderTargetEntity
        ) : viewportEntity_(viewportEntity),
            renderTargetEntity_(renderTargetEntity) {

            viewportEntity_.template getOrAdd<ClearComponent>(ClearFlags::Color);
            viewportEntity_.template getOrAdd<RenderTargetBindingComponent>(renderTargetEntity_.handle());
        }

    public:

        Viewport& render(
            Scene<TRenderHandles>& scene,
            Camera<TRenderHandles>& camera) {

            viewportEntity_.template getOrAdd<SceneBindingComponent>(scene.handle());
            viewportEntity_.template getOrAdd<CameraBindingComponent>(camera.handle());

            return *this;
        }

        Viewport& setRect(helios::math::vec4f rect) noexcept {
            auto& rectComp = viewportEntity_.template getOrAdd<RectComponent>();
            viewportEntity_.track(&rectComp)->setValue(rect);

            return *this;
        }

        Viewport& setClearFlags(ClearFlags flags) noexcept {
            auto& clearComp = viewportEntity_.template getOrAdd<ClearComponent>();
            viewportEntity_.track(&clearComp)->flags = flags;
            return *this;
        }

        Viewport& setBackgroundColor(helios::math::vec4f color) noexcept {
            auto& colorComp = viewportEntity_.template getOrAdd<ColorComponent>();
            viewportEntity_.track(&colorComp)->setValue(color);

            return *this;
        }

        ViewportEntity entity() const noexcept {
            return viewportEntity_;
        }

        ViewportHandle handle() const noexcept {
            return viewportEntity_.handle();
        }

    };

}