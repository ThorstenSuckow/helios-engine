module;

#include <cstdint>
#include <cstddef>
#include <functional>

export module helios.engine.rendering.vertexInput.types:VertexInputKey;

export namespace helios::engine::rendering::vertexInput::types {

    /**
     * @brief Identifies the vertex input configuration for a (shader, mesh) pair.
     */
    struct VertexInputKey {

        std::uint64_t shaderUid;
        std::uint64_t meshUid;

        bool operator==(const VertexInputKey&) const = default;
    };

}