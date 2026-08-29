/**
 * @file WarmupDoneSystem.ixx
 * @brief System that emits a game-state warmup-done transition when no active shader sources remain.
 */
module;

#include <concepts>

export module helios.engine.platform.lifecycle.systems.WarmupDoneSystem;


import helios.ecs;

import helios.ecs.command.types;

import helios.engine.runtime;
import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.common.Session;
import helios.engine.runtime.concepts;
import helios.engine.rendering;
import helios.engine.state;

using namespace helios::engine::runtime::enginestate::types;
using namespace helios::engine::rendering::shader::types;
using namespace helios::ecs::common::concepts;
using namespace helios::ecs::command;
using namespace helios::ecs::components;
using namespace helios::engine::rendering::shader::components;

using namespace helios::engine::runtime::world;
using namespace helios::ecs;
using namespace helios::engine::state::types;
using namespace helios::engine::state::commands;

export namespace helios::engine::platform::lifecycle::systems {

    /**
     * @brief Signals warmup completion through a typed state command buffer.
     */
    template<typename TRenderHandles>
    class WarmupDoneSystem {

        using ShaderHandle = typename TRenderHandles::ShaderHandle;
        using TextureHandle = typename TRenderHandles::TextureHandle;

    public:


        using CommandBuffer = ecs::command::TypedCommandBuffer<StateCommand<EngineState>>;

        /**
         * @brief Queues `StateCommand<EngineState>` with `WarmupDoneSignal` when warmup resources are consumed.
         *
         * @param updateContext Frame-local update context.
         */
        void update(UpdateContext& updateContext, runtime::common::Session& session, CommandBuffer& cmdBuffer) noexcept {

            if (updateContext.template view<
                ShaderHandle,
                ShaderSourceComponent<ShaderHandle>
                >().withActive().empty() &&
                updateContext.template view<
                TextureHandle,
                rendering::texture::components::TextureSourceComponent<TextureHandle>
                >().withActive().empty()
                ) {

                cmdBuffer.template add<StateCommand<EngineState>>(
                    StateTransitionRequest<EngineState>(
                        session.state<EngineState>(),
                        EngineStateTransitionId::WarmupDone
                    )
                );

            }

        }

    };



}