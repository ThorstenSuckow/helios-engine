module;

export module helios.engine.factory:Camera;

import helios.ecs.entity;
import helios.engine.rendering.common.components;
import helios.engine.core.components;
import helios.engine.core.types;
import helios.engine.spatial.components;
import helios.engine.scene.components;
import helios.math;

export namespace helios::engine::factory {

    template<typename TRenderHandles>
    class Camera {

        using CameraHandle = typename TRenderHandles::CameraHandle;

        using EntityManager = ecs::entity::EntityManager<CameraHandle>;
        using CameraEntity = ecs::entity::Entity<EntityManager>;

        CameraEntity cameraEntity_;

    public:

        explicit Camera(CameraEntity cameraEntity)
        : cameraEntity_(cameraEntity) {

            cameraEntity.template add<helios::engine::scene::components::ProjectionMatrixComponent>();
            cameraEntity.template add<helios::engine::scene::components::ViewMatrixComponent>();
            cameraEntity.template add<helios::engine::spatial::components::YawPitchRollComponent>();
            cameraEntity.template add<helios::engine::spatial::components::Rotation3DComponent<helios::engine::core::types::Local>>();
            cameraEntity.template add<helios::engine::spatial::components::TransformComponent<helios::engine::core::types::World>>(1.0F);
        }

        template<typename TType>
        Camera& setPosition(helios::math::vec3f position) {
            auto& posComp = cameraEntity_.template getOrAdd<helios::engine::spatial::components::Position3DComponent<TType>>();
            cameraEntity_.track(&posComp)->setValue(position);
            return *this;
        }

        Camera& setFovY(float fovYRadians) {
            auto& comp = cameraEntity_.template getOrAdd<helios::engine::scene::components::PerspectiveCameraComponent>();
            cameraEntity_.track(&comp)->setFovY(fovYRadians);
            return *this;
        }

        Camera& setAspectRatio(float aspectRatio) {
            auto& comp = cameraEntity_.template getOrAdd<helios::engine::scene::components::PerspectiveCameraComponent>();
            cameraEntity_.track(&comp)->setAspectRatio(aspectRatio);
            return *this;
        }

        CameraEntity entity() const noexcept {
            return cameraEntity_;
        }

        CameraHandle handle() const noexcept {
            return cameraEntity_.handle();
        }

    };

}

