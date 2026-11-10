export module helios.engine.factory:RenderTargetFactory;

import helios.ecs.entity.EntityWorld;
import :RenderTarget;

export namespace helios::engine::factory {

    class RenderTargetFactory {

        using EntityWorld = helios::ecs::entity::EntityWorld;

        EntityWorld& ecsWorld_;
    public:

        RenderTargetFactory(const RenderTargetFactory&) = delete;
        RenderTargetFactory& operator=(const RenderTargetFactory&) = delete;
        RenderTargetFactory(RenderTargetFactory&&) = delete;
        RenderTargetFactory& operator=(RenderTargetFactory&&) = delete;

        explicit RenderTargetFactory (EntityWorld& ecsWorld) : ecsWorld_(ecsWorld) {}

        template<typename TRenderHandles>
        RenderTarget<TRenderHandles> createRenderTarget() {
            auto entity = ecsWorld_.add<typename TRenderHandles::RenderTargetHandle>();
            entity.setActive(true);

            return RenderTarget<TRenderHandles>(entity);
        }

    };

}