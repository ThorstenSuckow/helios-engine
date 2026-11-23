module;


export module helios.engine.rendering.shader.components:ShaderMappingBindingComponent;

import helios.ecs.component.components;

export namespace helios::engine::rendering::shader::components {

    struct ShaderMappingBindingComponentTag;
    template<typename TShaderMappingHandle>
    using ShaderMappingBindingComponent = ecs::components::BindingComponent<TShaderMappingHandle, ShaderMappingBindingComponentTag>;

}