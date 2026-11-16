module;

#include <unordered_set>
#include <cassert>

export module helios.engine.rendering.vertexInput.systems:VertexInputSetupSystem;

import helios.ecs.entity;
import helios.engine.rendering.common.components;
import helios.engine.rendering.vertexInput.commands;
import helios.engine.rendering.vertexInput.components;
import helios.engine.rendering.common.types;

import helios.engine.rendering.shader.components;
import helios.engine.rendering.mesh.components;

import helios.ecs.command;


export namespace helios::engine::rendering::vertexInput::systems {

    template<typename TMemberHandle, typename TRenderHandles>
    class VertexInputSetupSystem {

        using MeshHandle = typename TRenderHandles::MeshHandle;
        using ShaderHandle = typename TRenderHandles::ShaderHandle;

        using VertexInputQuery = ecs::entity::query::Query<
            typename TMemberHandle,
            ecs::entity::ReadSet<
                mesh::components::MeshBindingComponent<MeshHandle>,
                shader::components::ShaderBindingComponent<ShaderHandle>
            >,
            ecs::entity::WriteSet<>,
            ecs::entity::query::Filter<ecs::entity::query::AnyDirty<
                mesh::components::MeshBindingComponent<MeshHandle>,
                shader::components::ShaderBindingComponent<ShaderHandle>
            >>,
            ecs::entity::query::Optional<
                components::InstanceDataLayoutComponent
        >>;

        using MeshQuery = ecs::entity::query::Query<
           typename TRenderHandles::ShaderHandle,
           ecs::entity::ReadSet<
               components::VertexBufferLayoutComponent
           >,
           ecs::entity::WriteSet<>
        >;

        using ShaderQuery = ecs::entity::query::Query<
            typename TRenderHandles::ShaderHandle,
            ecs::entity::ReadSet<
                shader::components::VertexAttributeMappingsComponent
            >,
            ecs::entity::WriteSet<>
        >;


        using CommandBuffer = ecs::command::TypedCommandBuffer<
            commands::VertexInputSetupCommand<TMemberHandle, TRenderHandles>
        >;

    public:


        void update(
            VertexInputQuery query,
            MeshQuery meshQuery,
            ShaderQuery shaderQuery,
            CommandBuffer& commandBuffer
        ) noexcept {

            for (auto [vertexInputEntity, mbc, sbc, vblc] : query) {

                auto meshHandle = mbc->targetHandle();
                auto shaderHandle = sbc->targetHandle();

                auto shaderResult = shaderQuery.get(shaderHandle);
                if (!shaderResult) {
                    assert(false && "Shader entity not found");
                    continue;
                }

                auto meshResult = meshQuery.get(meshHandle);
                if (!meshResult) {
                    assert(false && "Mesh entity not found");
                    continue;
                }

                auto [shaderEntity, mappingComponent] = *shaderResult;
                auto [meshEntity, layoutComponent] = *meshResult;

                commandBuffer.template add<commands::VertexInputSetupCommand<TMemberHandle, TRenderHandles>>(
                    vertexInputEntity.handle(),
                    meshHandle,
                    shaderHandle,
                    mappingComponent->mappings,
                    layoutComponent->vertexBufferLayout,
                    vblc ? vblc->vertexBufferLayout : types::VertexBufferLayout{}
                );

            }

        }

    };

}