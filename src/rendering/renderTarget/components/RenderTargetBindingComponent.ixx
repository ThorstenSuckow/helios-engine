/**
 * @file RenderTargetBindingComponent.ixx
 * @brief Binding component alias for renderTarget targets.
 */
module;


export module helios.engine.rendering.renderTarget.components.RenderTargetBindingComponent;

import helios.engine.rendering.renderTarget.types.RenderTargetHandle;
import helios.ecs.components.BindingComponent;

using namespace helios::engine::rendering::renderTarget::types;
using namespace helios::ecs::components;
export namespace helios::engine::rendering::renderTarget::components {

    /**
     * @brief Binds an owning entity to a renderTarget handle.
     *
     * @tparam TOwnerHandle Owning entity handle type.
     */
    template<typename TOwnerHandle>
    using RenderTargetBindingComponent = BindingComponent<TOwnerHandle, RenderTargetHandle>;

}