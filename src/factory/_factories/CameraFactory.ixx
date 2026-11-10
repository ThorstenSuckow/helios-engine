
export module helios.engine.factory:CameraFactory;

import helios.ecs.entity.EntityWorld;
import :Camera;
import helios.engine.scene.components;

export namespace helios::engine::factory {

    class CameraFactory {

        using EntityWorld = helios::ecs::entity::EntityWorld;

        template<typename TRenderHandles>
        using RenderTargetEntity = ecs::entity::Entity<ecs::entity::EntityManager<typename TRenderHandles::RenderTargetHandle>>;

        template<typename TRenderHandles>
        using CameraHandle = typename TRenderHandles::CameraHandle;

        using PerspectiveCameraComponent = helios::engine::scene::components::PerspectiveCameraComponent;

        EntityWorld& ecsWorld_;
    public:

        CameraFactory(const CameraFactory&) = delete;
        CameraFactory& operator=(const CameraFactory&) = delete;
        CameraFactory(CameraFactory&&) = delete;
        CameraFactory& operator=(CameraFactory&&) = delete;

        explicit CameraFactory (EntityWorld& ecsWorld) : ecsWorld_(ecsWorld) {}

        template<typename TRenderHandles>
        Camera<TRenderHandles> createPerspectiveCamera(const float fovYRadians, const float aspectRatio) {
            auto entity = ecsWorld_.add<CameraHandle<TRenderHandles>>();
            entity.template add<PerspectiveCameraComponent>(fovYRadians, aspectRatio);
            entity.setActive(true);

            return Camera<TRenderHandles>(entity);
        }


    };

}

