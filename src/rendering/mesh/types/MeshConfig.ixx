/**
 * @file MeshConfig.ixx
 * @brief Rendering configuration for mesh draw interpretation.
 */
module;

export module helios.engine.rendering.mesh.types.MeshConfig;

import helios.engine.rendering.mesh.types.PrimitiveType;


export namespace helios::engine::rendering::mesh::types {

    /**
     * @brief Rendering metadata that defines how mesh buffers are interpreted.
     */
    struct MeshConfig {

        /**
         * @brief Primitive topology used by draw calls.
         */
        PrimitiveType primitiveType = PrimitiveType::Triangles;

    };
}
