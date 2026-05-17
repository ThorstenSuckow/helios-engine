/**
 * @file FramebufferBindingComponent.ixx
 * @brief Binding component alias for framebuffer targets.
 */
module;


export module helios.engine.rendering.framebuffer.components.FramebufferBindingComponent;

import helios.engine.rendering.framebuffer.types.FramebufferHandle;
import helios.ecs.components.BindingComponent;

using namespace helios::engine::rendering::framebuffer::types;
using namespace helios::ecs::components;
export namespace helios::engine::rendering::framebuffer::components {

    /**
     * @brief Binds an owning entity to a framebuffer handle.
     *
     * @tparam TOwnerHandle Owning entity handle type.
     */
    template<typename TOwnerHandle>
    using FramebufferBindingComponent = BindingComponent<TOwnerHandle, FramebufferHandle>;

}