/**
* @file ShaderBindingComponent.ixx
 * @brief Binding component alias for shader targets.
 */
module;


export module helios.engine.rendering.mesh.components.MeshBindingComponent;

import helios.ecs.component;

using namespace helios::ecs::components;
export namespace helios::engine::rendering::mesh::components {

    struct MeshBindingComponentTag;
    template<typename TRenderHandles>
    using MeshBindingComponent = BindingComponent<typename TRenderHandles::MeshHandle, MeshBindingComponentTag>;

}