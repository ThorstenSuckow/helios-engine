
/**
 * @file ShaderBindingComponent.ixx
 * @brief Binding component alias for shader targets.
 */
module;


export module helios.engine.rendering.shader.components.ShaderBindingComponent;

import helios.ecs.component;

using namespace helios::ecs::components;
export namespace helios::engine::rendering::shader::components {

    struct ShaderBindingComponentTag;
    template<typename TRenderHandles>
    using ShaderBindingComponent = BindingComponent<typename TRenderHandles::ShaderHandle, ShaderBindingComponentTag>;

}