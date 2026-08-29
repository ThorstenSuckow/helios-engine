/**
 * @file SceneMemberComponent.ixx
 * @brief Binding component alias for scene membership.
 */
module;


export module helios.engine.scene.components.SceneMemberComponent;

import helios.ecs.component;

using namespace helios::ecs::components;
export namespace helios::engine::scene::components {

    struct SceneMemberComponentTag;
    template<typename TOwnerHandle, typename TRenderHandles>
    using SceneMemberComponent = BindingComponent<TOwnerHandle, typename TRenderHandles::SceneHandle, SceneMemberComponentTag>;

}