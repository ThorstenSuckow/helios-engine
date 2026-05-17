/**
 * @file FramebufferId.ixx
 * @brief Strongly-typed identifier for framebuffer resources.
 */
module;

export module helios.engine.rendering.framebuffer.types.FramebufferId;

import helios.ecs.types.StrongId;
import helios.engine.rendering.framebuffer.types.FramebufferDomainTag;

export namespace helios::engine::rendering::framebuffer::types {

    /**
     * @brief Strongly-typed identifier for framebuffer resources.
     */
    using FramebufferId = helios::ecs::types::StrongId<FramebufferDomainTag>;

}
