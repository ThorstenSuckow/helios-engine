/**
 * @file TextureBatchUploadCommand.ixx
 * @brief Command payload for uploading multiple texture resources.
 */
module;

#include <vector>

export module helios.engine.rendering.texture.commands:TextureBatchUploadCommand;

export namespace helios::engine::rendering::texture::commands {

    /**
     * @brief Requests batched upload for multiple texture handles.
     *
     * @tparam THandle Texture handle type.
     */
    template<typename THandle>
    struct TextureBatchUploadCommand {
        std::vector<THandle> textureHandles{};
    };

}