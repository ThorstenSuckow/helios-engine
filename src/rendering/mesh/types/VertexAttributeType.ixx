/**
 * @file VertexAttributeType.ixx
 * @brief Declares scalar storage types for vertex attributes.
 */
module;

export module helios.engine.rendering.mesh.types.VertexAttributeType;


export namespace helios::engine::rendering::mesh::types {

    /**
     * @brief Supported scalar component types in vertex attributes.
     */
    enum class VertexAttributeType {
        /**
         * @brief 32-bit floating-point component.
         */
        Float,

        /**
         * @brief 3-component float vector (`vec3`).
         */
        Vec3f,

        /**
         * @brief 4-component float vector (`vec4`).
         */
        Vec4f,

        /**
         * @brief 4x4 float matrix (`mat4`).
         */
        Mat4f
    };


}