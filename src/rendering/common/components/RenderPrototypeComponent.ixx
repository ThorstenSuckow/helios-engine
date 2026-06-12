/**
 * @file RenderPrototypeComponent.ixx
 * @brief Groups mesh, shader, and material handles into one render prototype.
 */
module;

export module helios.engine.rendering.common.components.RenderPrototypeComponent;

import helios.engine.rendering.mesh;
import helios.engine.rendering.shader;
import helios.engine.rendering.material;


using namespace helios::engine::rendering::mesh::types;
using namespace helios::engine::rendering::shader::types;
using namespace helios::engine::rendering::material::types;
using namespace helios::engine::rendering::mesh;
using namespace helios::engine::rendering::shader;
using namespace helios::engine::rendering::material;
export namespace helios::engine::rendering::common::components {

    /**
     * @brief Component that stores the resource handles needed for rendering.
     *
     * @tparam TOwnerHandle Owning entity handle type.
     */
    template<typename TOwnerHandle, typename TRenderMode>
    class RenderPrototypeComponent {

        ShaderHandle shaderHandle_;

        MaterialHandle materialHandle_;

        MeshHandle meshHandle_;

    public:

        /**
         * @brief Constructs the component from explicit resource handles.
         *
         * @param shaderHandle Shader handle.
         * @param materialHandle Material handle.
         * @param meshHandle Mesh handle.
         */
        explicit RenderPrototypeComponent(
            const ShaderHandle shaderHandle,
            const MaterialHandle materialHandle,
            const MeshHandle meshHandle
        )
            : shaderHandle_(shaderHandle),
              materialHandle_(materialHandle),
              meshHandle_(meshHandle) {}

        /**
         * @brief Constructs the component from resource entities.
         *
         * @param shader Shader entity.
         * @param material Material entity.
         * @param mesh Mesh entity.
         */
        explicit RenderPrototypeComponent(
            const ShaderEntity shader,
            const MaterialEntity material,
            const MeshEntity mesh
        )
            : shaderHandle_(shader.handle()),
              materialHandle_(material.handle()),
              meshHandle_(mesh.handle()) {}

        /**
         * @brief Returns the mesh handle.
         *
         * @return Mesh resource handle.
         */
        [[nodiscard]] MeshHandle meshHandle() const noexcept {
            return meshHandle_;
        }

        /**
         * @brief Returns the shader handle.
         *
         * @return Shader resource handle.
         */
        [[nodiscard]] ShaderHandle shaderHandle() const noexcept {
            return shaderHandle_;
        }

        /**
         * @brief Returns the material handle.
         *
         * @return Material resource handle.
         */
        [[nodiscard]] MaterialHandle materialHandle() const noexcept {
            return materialHandle_;
        }

        /**
         * @brief Sets the mesh handle.
         *
         * @param meshHandle Mesh resource handle.
         */
        void setMeshHandle(const MeshHandle meshHandle) noexcept {
            meshHandle_ = meshHandle;
        }

        /**
         * @brief Sets the shader handle.
         *
         * @param shaderHandle Shader resource handle.
         */
        void setShaderHandle(const ShaderHandle shaderHandle) noexcept {
            shaderHandle_ = shaderHandle;
        }

        /**
         * @brief Sets the material handle.
         *
         * @param materialHandle Material resource handle.
         */
        void setMaterialHandle(const MaterialHandle materialHandle) noexcept {
            materialHandle_ = materialHandle;
        }

    };
}