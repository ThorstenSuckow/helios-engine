/**
 * @file MeshUploadSystem.ixx
 * @brief Collects active shader resources and queues batch compile commands.
 */
module;

#include <vector>
#include "helios-engine-config.h"

export module helios.engine.rendering.mesh.systems.MeshUploadSystem;

import helios.engine.rendering.mesh.concepts;
import helios.engine.runtime.messaging.command.NullCommandBuffer;
import helios.engine.runtime.messaging.command.concepts.IsCommandBufferLike;
import helios.engine.rendering.mesh.components.MeshDataComponent;
import helios.engine.rendering.mesh.components.MeshUploadRequestComponent;
import helios.engine.rendering.mesh.commands.MeshBatchUploadCommand;
import helios.engine.runtime.world.tags.SystemRole;
import helios.engine.runtime.world.UpdateContext;

import helios.ecs.components.Active;

using namespace helios::engine::rendering::mesh::concepts;
using namespace helios::engine::rendering::mesh::commands;
using namespace helios::engine::rendering::mesh::components;
using namespace helios::engine::runtime::messaging::command;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::world::tags;
using namespace helios::engine::runtime::messaging::command::concepts;
using namespace helios::ecs::components;
export namespace helios::engine::rendering::mesh::systems {


    template<typename THandle, typename TCommandBuffer = NullCommandBuffer, size_t TCapacity = DEFAULT_MESH_POOL_CAPACITY>
    requires IsMeshHandle<THandle> && IsCommandBufferLike<TCommandBuffer>
    class MeshUploadSystem {

        std::vector<THandle> meshHandles_;

        size_t capacity_;

    public:

        using EngineRoleTag = TypedSystemRole;
        using CommandBuffer_type = TCommandBuffer;

        explicit MeshUploadSystem(size_t capacity = TCapacity) : capacity_(capacity) {
            meshHandles_.reserve(capacity);
        }

        /**
         * @brief Collects mesh handles and queues one batch upload command.
         *
         * @param updateContext Frame update context.
         */
        void update(UpdateContext& updateContext, TCommandBuffer& cmdBuffer) noexcept {

            for (auto [entity, mdc, murc, ac] : updateContext.view<
                THandle,
                MeshDataComponent<THandle>,
                MeshUploadRequestComponent<THandle>,
                Active<THandle>
            >().whereEnabled()) {
                meshHandles_.push_back(entity.handle());
            }

            cmdBuffer.template add<MeshBatchUploadCommand<THandle>>(std::move(meshHandles_));

            meshHandles_.clear();
            meshHandles_.reserve(capacity_);


        }

    };



}