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
import helios.ecs.command.types;

import helios.engine.runtime.gameloop.types;



import helios.engine.rendering.shader.components;
import helios.engine.rendering.shader.commands;

import helios.ecs.common.concepts;

import helios.ecs.component;


using namespace helios::engine::runtime;
using namespace helios::ecs;
using namespace helios::engine::rendering::shader;
using namespace helios::ecs::common::concepts;
using namespace helios::engine::rendering::shader::components;
using namespace helios::engine::rendering::shader::commands;
using namespace helios::ecs::components;
export namespace helios::engine::rendering::shader::systems {

    /**
     * @brief System that batches shader compile requests for active shader entities.
     *
     * @tparam THandle Shader handle type.
     * @tparam TCapacity Initial reserve size for the internal handle cache.
     */
    template<typename THandle>
    class ShaderCompileSystem {

        using UpdateContext = runtime::gameloop::types::UpdateContext;

        std::vector<THandle> shaderHandles_;

        size_t capacity_;

    public:

        using CommandBuffer = ecs::command::TypedCommandBuffer<ShaderBatchCompileCommand<THandle>>;

        explicit ShaderCompileSystem(size_t capacity = SHADER_INITIAL_STORAGE_CAPACITY) : capacity_(capacity) {
            shaderHandles_.reserve(capacity);
        }

        void update(UpdateContext& updateContext, CommandBuffer& cmdBuffer) noexcept {

            for (auto [entity, scc] : updateContext.template view<
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