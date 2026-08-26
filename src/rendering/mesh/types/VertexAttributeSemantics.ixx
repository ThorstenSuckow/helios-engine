/**
 * @file VertexAttributeSemantics.ixx
 * @brief Enumerates semantic meanings for mesh vertex attributes.
 */
module;

export module helios.engine.rendering.mesh.types.VertexAttributeSemantics;


export namespace helios::engine::rendering::mesh::types {


    /**
     * @brief Semantic slots understood by mesh/shader vertex pipelines.
     */
    enum class VertexAttributeSemantics {

        /**
         * @brief Local-space vertex position.
         */
        Position,

        /**
         * @brief Texture coordinates.
         */
        TextureCoordinates,

        /**
         * @brief Per-instance model matrix data.
         */
        InstancedModelMatrix,

        /**
         * @brief Per-instance normalized age data.
         */
        InstancedNormalizedAge,

        /**
         * @brief Number of semantics (sentinel, not a real semantic).
         */
        size_
    };


}