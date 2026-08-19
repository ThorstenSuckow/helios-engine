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
import helios.engine.runtime.world.types;
import helios.engine.runtime.concepts;
import helios.ecs.system.tags;

import helios.engine.rendering.shader.components;
import helios.engine.rendering.shader.concepts;
import helios.engine.rendering.shader.commands;

import helios.ecs.common.concepts;

import helios.ecs.component;


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
     * @tparam TCapacity Initial reserve size for the internal handle cache.
     */
    template<typename THandle>
    requires IsShaderHandle<THandle>
    class ShaderCompileSystem {

        std::vector<THandle> shaderHandles_;

        size_t capacity_;

    public:

        using EcsRoleTag = ecs::system::tags::TypedSystemRole;

        explicit ShaderCompileSystem(size_t capacity = SHADER_INITIAL_STORAGE_CAPACITY) : capacity_(capacity) {
            shaderHandles_.reserve(capacity);
        }

        template<typename TUpdateContextType, typename TCommandBuffer>
        requires runtime::concepts::ProvidesUpdateContext<TUpdateContextType, UpdateContext> &&
                 ecs::command::concepts::IsCommandBufferLike<TCommandBuffer>
        bool update(TUpdateContextType& updateCtx, TCommandBuffer& cmdBuffer) noexcept {

            auto& updateContext = updateCtx.updateContext();

            for (auto [entity, scc] : updateContext.template view<
                THandle,
                ShaderSourceComponent<THandle>
            >().withActive()) {
                shaderHandles_.push_back(entity.handle());
            }

            cmdBuffer.template add<ShaderBatchCompileCommand<THandle>>(std::move(shaderHandles_));

            shaderHandles_.clear();
            shaderHandles_.reserve(capacity_);


            return true;
        }

    };



}