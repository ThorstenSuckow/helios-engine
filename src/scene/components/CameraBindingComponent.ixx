/**
 * @file CameraBindingComponent.ixx
 * @brief Binding component alias for camera game objects.
 */
module;

export module helios.engine.scene.components.CameraBindingComponent;

import helios.engine.scene.types.CameraHandle;

import helios.ecs.component;

using namespace helios::engine::scene::types;
using namespace helios::ecs::components;
export namespace helios::engine::scene::components {

    /**
     * @brief Binds an owning entity to a camera game object handle.
     *
     * @tparam TOwnerHandle Owning entity handle type.
     */
    template<typename TOwnerHandle>
    using CameraBindingComponent = BindingComponent<TOwnerHandle, CameraHandle>;

}