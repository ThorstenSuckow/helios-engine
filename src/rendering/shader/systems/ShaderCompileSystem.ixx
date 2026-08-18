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
     * @tparam TCommandBuffer Command buffer type used for queued compile commands.
     * @tparam TCapacity Initial reserve size for the internal handle cache.
     * @tparam TUpdateContextType Type of the UpdateContext to use.
     */
    template<
        typename THandle,
        typename TCommandBuffer = ecs::command::NullCommandBuffer,
        typename TUpdateContextType = types::SystemUpdateContext
    >
    requires IsShaderHandle<THandle> &&
             ecs::command::concepts::IsCommandBufferLike<TCommandBuffer> &&
             runtime::concepts::ProvidesUpdateContext<TUpdateContextType, UpdateContext>
    class ShaderCompileSystem {

        std::vector<THandle> shaderHandles_;

        size_t capacity_;

    public:

        using EcsRoleTag = ecs::system::tags::TypedSystemRole;
        using CommandBuffer_type = TCommandBuffer;
        using UpdateContextType = TUpdateContextType;

        explicit ShaderCompileSystem(size_t capacity = SHADER_INITIAL_STORAGE_CAPACITY) : capacity_(capacity) {
            shaderHandles_.reserve(capacity);
        }

        /**
         * @brief Collects active shader handles and queues one batch compile command.
         *
         * @param updateCtx Frame update context.
         */
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