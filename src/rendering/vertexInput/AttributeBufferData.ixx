/**
 * @file AttributeBufferData.ixx
 * @brief Data structure for attribute buffer data.
 */
module;


#include <cstdint>


export module helios.engine.rendering.vertexInput.AttributeBufferData;

import helios.engine.rendering.vertexInput.types;
import helios.engine.rendering.shader.types;
import helios.engine.rendering.mesh.types;

import helios.ecs.common.types;


export namespace helios::engine::rendering::vertexInput {

    struct AttributeBufferData {

        using EntityHandleValue = helios::ecs::common::types::EntityHandleValue;

        mesh::types::MeshData* meshData = nullptr;
        types::VertexBufferLayout* vertexBufferLayout = nullptr;
        types::InstanceBufferLayout* instanceBufferLayout = nullptr;
        shader::types::ShaderAttributeMappings* shaderAttributeMappings = nullptr;

        EntityHandleValue meshHandleValue;
        EntityHandleValue shaderMappingHandleValue;
        EntityHandleValue instanceBufferLayoutHandleValue;

    };


}