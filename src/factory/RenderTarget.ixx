module;

export module helios.engine.factory:RenderTarget;

import helios.ecs.entity;

import helios.engine.rendering.common.components;
import helios.engine.spatial.components;
import helios.engine.core.components;
import helios.engine.rendering.common.types;

export namespace helios::engine::factory {


    template<typename TRenderHandles>
    class RenderTarget {

        using RenderTargetHandle = typename TRenderHandles::RenderTargetHandle;
        using EntityManager = ecs::entity::EntityManager<RenderTargetHandle>;
        using RenderTargetEntity = ecs::entity::Entity<EntityManager>;

        using Size2DComponent = helios::engine::spatial::components::Size2DComponent;
        using ClearComponent = helios::engine::rendering::common::components::ClearComponent;
        using ColorComponent = helios::engine::core::components::ColorComponent;
        using ClearFlags = helios::engine::rendering::common::types::ClearFlags;
        using Colors = helios::engine::rendering::common::types::Colors;
        RenderTargetEntity renderTargetEntity_;

    public:

        explicit RenderTarget(RenderTargetEntity renderTargetEntity)
        : renderTargetEntity_(renderTargetEntity) {

            renderTargetEntity_.template getOrAdd<Size2DComponent>();
            renderTargetEntity_.template getOrAdd<ClearComponent>(ClearFlags::Color);
            renderTargetEntity_.template getOrAdd<ColorComponent>(Colors::Black);
        }


        RenderTargetEntity entity() const noexcept {
            return renderTargetEntity_;
        }

        RenderTargetHandle handle() const noexcept {
            return renderTargetEntity_.handle();
        }

    };

}