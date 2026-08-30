/**
 * @file MeshUploadSystem.ixx
 * @brief Collects active shader resources and queues batch compile commands.
 */
module;

#include <vector>
#include "helios-engine-config.h"

export module helios.engine.rendering.mesh.systems.MeshUploadSystem;

import helios.ecs.command.NullCommandBuffer;
import helios.ecs.command;
import helios.engine.rendering.mesh.components;
import helios.engine.rendering.mesh.commands;

import helios.engine.runtime.gameloop.types;


import helios.ecs.component;
import helios.ecs.EcsWorld;

using namespace helios::engine::rendering::mesh::commands;
using namespace helios::engine::rendering::mesh::components;
using namespace helios::ecs;


using namespace helios::ecs::command::concepts;
using namespace helios::ecs::command;
using namespace helios::ecs::components;
export namespace helios::engine::rendering::mesh::systems {


    template<typename THandle>
    class MeshUploadSystem {

        using EcsWorld = ecs::EcsWorld;

        std::vector<THandle> meshHandles_;

        size_t capacity_;

    public:

        using CommandBuffer = ecs::command::TypedCommandBuffer<MeshBatchUploadCommand<THandle>>;

        explicit MeshUploadSystem(size_t capacity = MESH_INITIAL_STORAGE_CAPACITY) : capacity_(capacity) {
            meshHandles_.reserve(capacity);
        }

        void update(EcsWorld& ecsWorld, CommandBuffer& cmdBuffer) noexcept {

            for (auto [entity, mdc, murc] : ecsWorld.view<
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