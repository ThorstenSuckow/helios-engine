/**
 * @file SceneBindingComponent.ixx
 * @brief Binding component alias for scene handles.
 */
module;


export module helios.engine.scene.components.SceneBindingComponent;

import helios.ecs.component;

using namespace helios::ecs::components;
export namespace helios::engine::scene::components {

    struct SceneBindingComponentTag;
    template<typename TOwnerHandle, typename TRenderHandles>
    using SceneBindingComponent = BindingComponent<TOwnerHandle, typename TRenderHandles::SceneHandle, SceneBindingComponentTag>;

}