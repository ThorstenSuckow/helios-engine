/**
 * @file MeshBatchUploadCommand.ixx
 * @brief Command that requests uploading a batch of mesh handles.
 */
module;

#include <vector>

export module helios.engine.rendering.mesh.commands.MeshBatchUploadCommand;

import helios.engine.rendering.mesh.types.MeshDataIndex;
import helios.engine.rendering.mesh.types.MeshHandle;
import helios.engine.rendering.mesh.concepts.IsMeshHandle;

using namespace helios::engine::rendering::mesh::types;
using namespace helios::engine::rendering::mesh::concepts;
export namespace helios::engine::rendering::mesh::commands {

    /**
     * @brief Batch command containing mesh handles to upload.
     * @tparam THandle Handle type constrained to mesh handles.
     */
    template<typename THandle>
    requires IsMeshHandle<THandle>
    struct MeshBatchUploadCommand {

        /**
         * @brief Handles of meshes that should be uploaded.
         */
        std::vector<MeshHandle> meshHandles{};

    };

}