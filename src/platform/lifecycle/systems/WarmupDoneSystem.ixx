/**
 * @file WarmupDoneSystem.ixx
 * @brief System that emits a game-state warmup-done transition when no active shader sources remain.
 */
module;

#include <concepts>

export module helios.engine.platform.lifecycle.systems.WarmupDoneSystem;


import helios.engine.runtime.world.UpdateContext;

import helios.engine.runtime.world.tags.SystemRole;

import helios.engine.runtime.messaging.command;

import helios.engine.runtime.concepts;

import helios.engine.runtime.world;
import helios.ecs.components.Active;
import helios.engine.rendering.shader.concepts;

import helios.engine.rendering.shader.components;

import helios.engine.state.commands;
import helios.engine.state.types;

import helios.engine.runtime.enginestate;

using namespace helios::engine::runtime::enginestate::types;
using namespace helios::engine::rendering::shader::concepts;
using namespace helios::engine::runtime::messaging::command::concepts;
using namespace helios::ecs::components;
using namespace helios::engine::rendering::shader::components;
using namespace helios::engine::runtime::world::tags;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::messaging::command;
using namespace helios::engine::state::types;
using namespace helios::engine::state::commands;

export namespace helios::engine::platform::lifecycle::systems {

    /**
     * @brief Signals warmup completion through a typed state command buffer.
     *
     * @tparam THandle Shader handle type.
     * @tparam TCommandBuffer Command buffer type used to queue state commands.
     */
    template<typename THandle, typename TCommandBuffer = NullCommandBuffer>
    requires IsShaderHandle<THandle> &&
             IsCommandBufferLike<TCommandBuffer> &&
             (!std::is_same_v<TCommandBuffer, NullCommandBuffer>)
    class WarmupDoneSystem {

    public:

        using CommandBuffer_type = TCommandBuffer;

        /**
         * @brief Engine role marker used by runtime registries.
         */
        using EngineRoleTag = TypedSystemRole;

        /**
         * @brief Queues `StateCommand<EngineState>` with `WarmupDoneSignal` when warmup resources are consumed.
         *
         * @param updateContext Frame-local update context.
         */
        void update(UpdateContext& updateContext, TCommandBuffer& cmdBuffer) noexcept {

            if (updateContext.view<
                THandle,
                ShaderSourceComponent<THandle>,
                Active<THandle>
                >().whereEnabled().empty()) {

                cmdBuffer.template add<StateCommand<EngineState>>(
                    StateTransitionRequest<EngineState>(
                        updateContext.session().state<EngineState>(),
                        EngineStateTransitionId::WarmupDone
                    )
                );

            }


        }

    };



}