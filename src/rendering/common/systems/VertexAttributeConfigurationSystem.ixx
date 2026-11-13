module;

#include <unordered_set>
#include <cassert>

export module helios.engine.rendering.common.systems.VertexAttributeConfigurationSystem;

import helios.ecs.entity;
import helios.engine.rendering.common.components;
import helios.engine.rendering.common.commands;

import helios.engine.rendering.shader.components;
import helios.engine.rendering.mesh.components;

import helios.ecs.command;


export namespace helios::engine::rendering::common::systems {

    template<typename TMemberHandle, typename TSubmissionMode, typename TRenderHandles>
    class VertexAttributeConfigurationSystem {

        using MeshHandle = typename TRenderHandles::MeshHandle;
        using ShaderHandle = typename TRenderHandles::ShaderHandle;

        using RenderPrototypeQuery = ecs::entity::query::Query<
            TMemberHandle,
            ecs::entity::ReadSet<
                components::RenderPrototypeComponent<TSubmissionMode, TRenderHandles>
            >,
            ecs::entity::WriteSet<>,
            ecs::entity::query::Filter<ecs::entity::query::AnyDirty<
                components::RenderPrototypeComponent<TSubmissionMode, TRenderHandles>
            >>
        >;

        using MeshQuery = ecs::entity::query::Query<
            typename TRenderHandles::MeshHandle,
            ecs::entity::ReadSet<
                mesh::components::VertexAttributeLayoutsComponent
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
            commands::VertexAttributeConfigurationCommand<TRenderHandles>
        >;

        using MeshShaderPair = std::pair<MeshHandle, ShaderHandle>;

        struct MeshShaderPairHash {
            std::size_t operator()(const MeshShaderPair& pair) const noexcept {
                const auto h1 = std::hash<MeshHandle>{}(pair.first);
                const auto h2 = std::hash<ShaderHandle>{}(pair.second);
                /**
                 * @see https://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c11
                 */
                return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
            }
        };


        std::unordered_set<MeshShaderPair, MeshShaderPairHash> meshShaderPairs_;

    public:


        void update(
            RenderPrototypeQuery query,
            MeshQuery meshQuery,
            ShaderQuery shaderQuery,
            CommandBuffer& commandBuffer
        ) noexcept {

            meshShaderPairs_.clear();

            for (auto [queryEntity, renderPrototype] : query) {

                auto meshHandle = renderPrototype->meshHandle();
                auto shaderHandle = renderPrototype->shaderHandle();

                if (auto pair = MeshShaderPair{meshHandle, shaderHandle};
                    !meshShaderPairs_.insert(pair).second) {
                    continue;
                }

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

                commandBuffer.template add<commands::VertexAttributeConfigurationCommand<TRenderHandles>>(
                    meshHandle,
                    mappingComponent->mappings,
                    layoutComponent->layouts
                );

            }

        }

    };

}