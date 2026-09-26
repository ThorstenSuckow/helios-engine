/**
 * @file RenderTargetBindingComponent.ixx
 * @brief Binding component alias for renderTarget targets.
 */
module;


export module helios.engine.rendering.common.components.RenderTargetBindingComponent;

import helios.ecs.component;

using namespace helios::ecs::components;
export namespace helios::engine::rendering::common::components {

    struct RenderTargetBindingComponentTag;
    template<typename TRenderHandles>
    using RenderTargetBindingComponent = BindingComponent<typename TRenderHandles::RenderTargetHandle, RenderTargetBindingComponentTag>;

}