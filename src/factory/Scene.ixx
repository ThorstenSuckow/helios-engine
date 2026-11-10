module;

export module helios.engine.factory:Scene;

import :Camera;
import :CameraFactory;

import helios.ecs.entity;
import helios.ecs.entity.EntityWorld;

import helios.engine.scene.components;

export namespace helios::engine::factory {


    template<typename TRenderHandles>
    class Scene {

        using SceneHandle = typename TRenderHandles::SceneHandle;
        using EntityManager = ecs::entity::EntityManager<SceneHandle>;
        using SceneEntity = ecs::entity::Entity<EntityManager>;

        using CameraHandle = typename TRenderHandles::CameraHandle;
        using CameraEntityManager = ecs::entity::EntityManager<CameraHandle>;
        using CameraEntity = ecs::entity::Entity<CameraEntityManager>;

        using SceneBindingComponent = engine::scene::components::SceneBindingComponent<TRenderHandles>;


        SceneEntity sceneEntity_;

        // lives as long as the EntityWorld; factories are created on demand
        helios::ecs::entity::EntityWorld& ecsWorld_;


    public:

        explicit Scene(helios::ecs::entity::EntityWorld& ecsWorld, SceneEntity sceneEntity)
        : sceneEntity_(sceneEntity), ecsWorld_(ecsWorld) {}


        Camera<TRenderHandles> addPerspectiveCamera(const float fovYRadians, const float aspectRatio) {
            auto camera = CameraFactory(ecsWorld_).createPerspectiveCamera<TRenderHandles>(fovYRadians, aspectRatio);

            camera.entity().template add<SceneBindingComponent>(sceneEntity_.handle());
            return camera;
        }

        SceneEntity entity() const noexcept {
            return sceneEntity_;
        }

        SceneHandle handle() const noexcept {
            return sceneEntity_.handle();
        }

    };

}