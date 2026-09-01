/**
 * @file RenderPrototypeComponent.ixx
 * @brief Groups mesh, shader, and material handles into one render prototype.
 */
module;

export module helios.engine.rendering.common.components.RenderPrototypeComponent;

import helios.ecs;

export namespace helios::engine::rendering::common::components {

    /**
     * @brief Component that stores the resource handles needed for rendering.
     *
     * @tparam TOwnerHandle Owning entity handle type.
     */
    template<typename TOwnerHandle, typename TSubmissionMode, typename TRenderHandles>
    class RenderPrototypeComponent {

        using ShaderHandle = typename TRenderHandles::ShaderHandle;
        using MaterialHandle = typename TRenderHandles::MaterialHandle;
        using MeshHandle = typename TRenderHandles::MeshHandle;
        using TextureHandle = typename TRenderHandles::TextureHandle;

        ShaderHandle shaderHandle_;
        MaterialHandle materialHandle_;
        MeshHandle meshHandle_;
        TextureHandle textureHandle_;

        using ShaderEntity = ecs::entity::Entity<ecs::entity::EntityManager<ShaderHandle>>;
        using MaterialEntity = ecs::entity::Entity<ecs::entity::EntityManager<MaterialHandle>>;
        using MeshEntity = ecs::entity::Entity<ecs::entity::EntityManager<MeshHandle>>;
        using TextureEntity = ecs::entity::Entity<ecs::entity::EntityManager<TextureHandle>>;

    public:
        using HandleType = TOwnerHandle;
        /**
         * @brief Constructs the component from explicit resource handles.
         *
         * @param shaderHandle Shader handle.
         * @param materialHandle Material handle.
         * @param meshHandle Mesh handle.
         * @param textureHandle
         */
        explicit RenderPrototypeComponent(
            const ShaderHandle shaderHandle,
            const MaterialHandle materialHandle,
            const MeshHandle meshHandle,
            const TextureHandle textureHandle = {}
        )
            : shaderHandle_(shaderHandle),
              materialHandle_(materialHandle),
              meshHandle_(meshHandle),
              textureHandle_(textureHandle) {}

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
            const MeshEntity mesh,
            const TextureEntity texture
        )
            : shaderHandle_(shader.handle()),
              materialHandle_(material.handle()),
              meshHandle_(mesh.handle()),
              textureHandle_(texture.handle()) {}

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
         * @brief Returns the texture handle.
         *
         * @return Texture resource handle.
         */
        [[nodiscard]] TextureHandle textureHandle() const noexcept {
            return textureHandle_;
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

        /**
         * @brief Sets the texture handle.
         *
         * @param textureHandle Texture resource handle.
         */
        void setTextureHandle(const TextureHandle textureHandle) noexcept {
            textureHandle_ = textureHandle;
        }

    };
}