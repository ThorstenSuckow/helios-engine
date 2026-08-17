/**
 * @file MeshUploadSystem.ixx
 * @brief Collects active shader resources and queues batch compile commands.
 */
module;

#include <vector>
#include "helios-engine-config.h"

export module helios.engine.rendering.mesh.systems.MeshUploadSystem;

import helios.engine.rendering.mesh.concepts;
import helios.ecs.command.NullCommandBuffer;
import helios.ecs.command.concepts;
import helios.engine.rendering.mesh.components;
import helios.engine.rendering.mesh.commands;
import helios.engine.runtime.world.tags.SystemRole;
import helios.engine.runtime.world.UpdateContext;

import helios.ecs.component;

using namespace helios::engine::rendering::mesh::concepts;
using namespace helios::engine::rendering::mesh::commands;
using namespace helios::engine::rendering::mesh::components;
using namespace helios::ecs;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::world::tags;
using namespace helios::ecs::command::concepts;
using namespace helios::ecs::command;
using namespace helios::ecs::components;
export namespace helios::engine::rendering::mesh::systems {


    template<typename THandle, typename TCommandBuffer = ecs::command::NullCommandBuffer>
    requires IsMeshHandle<THandle> && ecs::command::concepts::IsCommandBufferLike<TCommandBuffer>
    class MeshUploadSystem {

        std::vector<THandle> meshHandles_;

        size_t capacity_;

    public:

        using EcsRoleTag = TypedSystemRole;
        using CommandBuffer_type = TCommandBuffer;

        explicit MeshUploadSystem(size_t capacity = MESH_INITIAL_STORAGE_CAPACITY) : capacity_(capacity) {
            meshHandles_.reserve(capacity);
        }

        /**
         * @brief Collects mesh handles and queues one batch upload command.
         *
         * @param updateContext Frame update context.
         */
        void update(UpdateContext& updateContext, TCommandBuffer& cmdBuffer) noexcept {

            for (auto [entity, mdc, murc] : updateContext.view<
                THandle,
                MeshDataComponent<THandle>,
                MeshUploadRequestComponent<THandle>
            >().withActive()) {
                meshHandles_.push_back(entity.handle());
            }

            cmdBuffer.template add<MeshBatchUploadCommand<THandle>>(std::move(meshHandles_));

            meshHandles_.clear();
            meshHandles_.reserve(capacity_);


        }

    };



}