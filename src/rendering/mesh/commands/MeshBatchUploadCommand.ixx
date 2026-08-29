/**
 * @file MeshBatchUploadCommand.ixx
 * @brief Command that requests uploading a batch of mesh handles.
 */
module;

#include <vector>

export module helios.engine.rendering.mesh.commands.MeshBatchUploadCommand;

import helios.engine.rendering.mesh.types.MeshDataIndex;

using namespace helios::engine::rendering::mesh::types;
export namespace helios::engine::rendering::mesh::commands {

    /**
     * @brief Batch command containing mesh handles to upload.
     * @tparam THandle Handle type constrained to mesh handles.
     */
    template<typename THandle>
    struct MeshBatchUploadCommand {

        /**
         * @brief Handles of meshes that should be uploaded.
         */
        std::vector<THandle> meshHandles{};

    };

}