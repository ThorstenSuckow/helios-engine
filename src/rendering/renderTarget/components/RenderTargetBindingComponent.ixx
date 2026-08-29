/**
 * @file RenderTargetBindingComponent.ixx
 * @brief Binding component alias for renderTarget targets.
 */
module;


export module helios.engine.rendering.renderTarget.components.RenderTargetBindingComponent;

import helios.ecs.component;

using namespace helios::ecs::components;
export namespace helios::engine::rendering::renderTarget::components {

    struct RenderTargetBindingComponentTag;
    template<typename TOwnerHandle, typename TRenderTargetHandle>
    using RenderTargetBindingComponent = BindingComponent<TOwnerHandle, TRenderTargetHandle, RenderTargetBindingComponentTag>;

}