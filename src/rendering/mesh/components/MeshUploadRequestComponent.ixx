/**
 * @file MeshUploadRequestComponent.ixx
 * @brief Tag component that marks an entity for mesh upload processing.
 */
module;

export module helios.engine.rendering.mesh.components.MeshUploadRequestComponent;

export namespace helios::engine::rendering::mesh::components {

    /**
     * @brief Tag component used to request mesh upload for an entity.
     * @tparam TWOwnerHandle Owner handle type used by ECS composition.
     */
    template<typename TWOwnerHandle>
    struct MeshUploadRequestComponent {};


}