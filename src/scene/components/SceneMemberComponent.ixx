/**
 * @file SceneMemberComponent.ixx
 * @brief Binding component alias for scene membership.
 */
module;


export module helios.engine.scene.components.SceneMemberComponent;

import helios.engine.scene.types.SceneHandle;

import helios.ecs.components.BindingComponent;

using namespace helios::engine::scene::types;
using namespace helios::ecs::components;
export namespace helios::engine::scene::components {

    /**
     * @brief Marks an owning entity as a member of a scene.
     *
     * @tparam TOwnerHandle Owning entity handle type.
     */
    template<typename TOwnerHandle>
    using SceneMemberComponent = BindingComponent<TOwnerHandle, SceneHandle>;

}