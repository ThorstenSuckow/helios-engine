/**
 * @file CameraBindingComponent.ixx
 * @brief Binding component alias for camera game objects.
 */
module;

export module helios.engine.scene.components.CameraBindingComponent;

import helios.ecs.component;

using namespace helios::ecs::components;
export namespace helios::engine::scene::components {

    struct CameraBindingComponentTag;
    template<typename TOwnerHandle, typename TTargetHandle>
    using CameraBindingComponent = BindingComponent<TOwnerHandle, TTargetHandle, CameraBindingComponentTag>;

}