/**
 * @file FramebufferHandle.ixx
 * @brief Entity-handle alias for framebuffer resources.
 */
module;

export module helios.engine.rendering.framebuffer.types.FramebufferHandle;


import helios.ecs.types;
import helios.engine.rendering.framebuffer.types.FramebufferDomainTag;

export namespace helios::engine::rendering::framebuffer::types {

    /**
     * @brief Handle type used to reference framebuffer entities.
     */
    using FramebufferHandle = helios::ecs::types::EntityHandle<FramebufferDomainTag>;

}
