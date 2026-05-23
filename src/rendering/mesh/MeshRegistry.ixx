/**
 * @file MeshRegistry.ixx
 * @brief Registry that stores mesh data entries and provides index-based access.
 */
module;

#include <vector>
#include "helios-engine-config.h"
#include <cassert>

export module helios.engine.rendering.mesh.MeshRegistry;

import helios.engine.rendering.mesh.types.MeshData;
import helios.engine.rendering.mesh.types.MeshDataIndex;

using namespace helios::engine::rendering::mesh::types;
export namespace helios::engine::rendering::mesh {


    /**
     * @brief Stores mesh data records used by rendering systems.
     */
    class MeshRegistry {

        std::vector<MeshData> meshes_;


    public:

        /**
         * @brief Constructs the registry and reserves default mesh capacity.
         */
        MeshRegistry() {
            meshes_.reserve(DEFAULT_MESH_POOL_CAPACITY);
        };

        /**
         * @brief Registers mesh data and returns its stable index.
         *
         * @tparam Args Constructor argument types forwarded to `MeshData`.
         * @param args Constructor arguments forwarded into the stored mesh data.
         * @return Index of the newly registered mesh data entry.
         */
        template<typename... Args>
        [[nodiscard]] MeshDataIndex registerMeshData(Args&&... args) {
            meshes_.emplace_back(std::forward<Args>(args)...);
            return meshes_.size() - 1;
        }

        /**
         * @brief Returns mesh data by index.
         *
         * @param meshIdx Index of the mesh data entry.
         * @return Const reference to the stored mesh data.
         */
        [[nodiscard]] const MeshData& meshData(const MeshDataIndex meshIdx) const {
            assert(meshIdx < meshes_.size() && "meshId out of bounds for meshes.");
            return meshes_.at(meshIdx);
        }

    };

}