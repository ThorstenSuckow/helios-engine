export module helios.engine.factory:SceneFactory;

import helios.ecs.entity.EntityWorld;
import :Scene;


export namespace helios::engine::factory {

    class SceneFactory {

        using EntityWorld = helios::ecs::entity::EntityWorld;

        template<typename TRenderHandles>
        using RenderTargetEntity = ecs::entity::Entity<ecs::entity::EntityManager<typename TRenderHandles::RenderTargetHandle>>;

        template<typename TRenderHandles>
        using ViewportHandle = typename TRenderHandles::ViewportHandle;

        EntityWorld& ecsWorld_;
    public:

        SceneFactory(const SceneFactory&) = delete;
        SceneFactory& operator=(const SceneFactory&) = delete;
        SceneFactory(SceneFactory&&) = delete;
        SceneFactory& operator=(SceneFactory&&) = delete;

        explicit SceneFactory (EntityWorld& ecsWorld) : ecsWorld_(ecsWorld) {}

        template<typename TRenderHandles>
        Scene<TRenderHandles> createScene() {
            auto entity = ecsWorld_.add<typename TRenderHandles::SceneHandle>();
            entity.setActive(true);

            return Scene<TRenderHandles>(ecsWorld_, entity);
        }


    };

}