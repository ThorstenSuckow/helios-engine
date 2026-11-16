/**
 * @file VertexInputRate.ixx
 * @brief Marker types for vertex input stepping behavior.
 */
module;


export module helios.engine.rendering.vertexInput.types:VertexInputRate;

export namespace helios::engine::rendering::vertexInput::types {

    enum class VertexInputRate{
        PerVertex,
        PerInstance
    };

    struct PerVertex{};
    struct PerInstance{};


}