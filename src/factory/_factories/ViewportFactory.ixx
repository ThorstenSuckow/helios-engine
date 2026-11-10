export module helios.engine.factory:ViewportFactory;

import helios.ecs.entity.EntityWorld;
import :Viewport;


export namespace helios::engine::factory {

    class ViewportFactory {

        using EntityWorld = helios::ecs::entity::EntityWorld;

        template<typename TRenderHandles>
        using RenderTargetEntity = ecs::entity::Entity<ecs::entity::EntityManager<typename TRenderHandles::RenderTargetHandle>>;

        template<typename TRenderHandles>
        using ViewportHandle = typename TRenderHandles::ViewportHandle;

        EntityWorld& ecsWorld_;
    public:

        ViewportFactory(const ViewportFactory&) = delete;
        ViewportFactory& operator=(const ViewportFactory&) = delete;
        ViewportFactory(ViewportFactory&&) = delete;
        ViewportFactory& operator=(ViewportFactory&&) = delete;

        explicit ViewportFactory (EntityWorld& ecsWorld) : ecsWorld_(ecsWorld) {}

        template<typename TRenderHandles>
        Viewport<TRenderHandles> createViewport(
            const helios::math::vec4f rect,
            RenderTargetEntity<TRenderHandles> renderTargetEntity
        ) {
            auto entity = ecsWorld_.add<ViewportHandle<TRenderHandles>>();
            entity.setActive(true);

            auto vp = Viewport<TRenderHandles>(
                entity, renderTargetEntity
            );
            vp.setRect(rect);
            return vp;
        }


    };

}