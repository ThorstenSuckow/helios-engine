/**
 * @file ShaderCompileSystem.ixx
 * @brief Collects active shader resources and queues batch compile commands.
 */
module;

#include <span>
#include <vector>
#include <cstddef>
#include "helios-engine-config.h"

export module helios.engine.rendering.shader.systems.ShaderCompileSystem;

import helios.ecs.command;

import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.tags.SystemRole;

import helios.engine.rendering.shader.components;
import helios.engine.rendering.shader.concepts;
import helios.engine.rendering.shader.commands;

import helios.ecs.common.concepts;

import helios.ecs.component;

using namespace helios::engine::runtime::world::tags;
using namespace helios::engine::runtime::world;
using namespace helios::ecs;
using namespace helios::engine::rendering::shader;
using namespace helios::ecs::common::concepts;
using namespace helios::engine::rendering::shader::components;
using namespace helios::engine::rendering::shader::commands;
using namespace helios::ecs::components;
using namespace helios::engine::rendering::shader::concepts;
export namespace helios::engine::rendering::shader::systems {

    /**
     * @brief System that batches shader compile requests for active shader entities.
     *
     * @tparam THandle Shader handle type.
     * @tparam TCommandBuffer Command buffer type used for queued compile commands.
     * @tparam TCapacity Initial reserve size for the internal handle cache.
     */
    template<typename THandle, typename TCommandBuffer = ecs::command::NullCommandBuffer>
    requires IsShaderHandle<THandle> && ecs::command::concepts::IsCommandBufferLike<TCommandBuffer>
    class ShaderCompileSystem {

        std::vector<THandle> shaderHandles_;

        size_t capacity_;

    public:

        using EcsRoleTag = TypedSystemRole;
        using CommandBuffer_type = TCommandBuffer;

        explicit ShaderCompileSystem(size_t capacity = SHADER_INITIAL_STORAGE_CAPACITY) : capacity_(capacity) {
            shaderHandles_.reserve(capacity);
        }

        /**
         * @brief Collects active shader handles and queues one batch compile command.
         *
         * @param updateContext Frame update context.
         */
        void update(UpdateContext& updateContext, TCommandBuffer& cmdBuffer) noexcept {

            for (auto [entity, scc] : updateContext.view<
                THandle,
                ShaderSourceComponent<THandle>
            >().withActive()) {
                shaderHandles_.push_back(entity.handle());
            }

            cmdBuffer.template add<ShaderBatchCompileCommand<THandle>>(std::move(shaderHandles_));

            shaderHandles_.clear();
            shaderHandles_.reserve(capacity_);


        }

    };



}