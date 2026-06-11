/**
 * @file vertexInputRates.ixx
 * @brief Marker types for vertex input stepping behavior.
 */
module;


export module helios.engine.rendering.mesh.types.vertexInputRates;

export namespace helios::engine::rendering::mesh::types {

    /**
     * @brief Input advances once per vertex.
     */
    struct PerVertex {};

    /**
     * @brief Input advances per instance.
     */
    struct PerInstance {};



}