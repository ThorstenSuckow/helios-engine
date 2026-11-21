/**
 * @file InstanceBufferLayoutComponent.ixx
 * @brief Component that stores active instance buffer layouts.
 */
module;

export module helios.engine.rendering.vertexInput.components:InstanceBufferLayoutComponent;

import helios.engine.rendering.vertexInput.types;

export namespace helios::engine::rendering::vertexInput::components {

    struct InstanceBufferLayoutComponent {
        types::InstanceBufferLayout instanceBufferLayout;
    };

}