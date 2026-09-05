/**
 * @file WarmupDoneSystem.ixx
 * @brief System that emits a game-state warmup-done transition when no active shader sources remain.
 */
module;

#include <concepts>

export module helios.engine.platform.lifecycle.systems.WarmupDoneSystem;


import helios.ecs;
import helios.ecs.entity.EntityWorld;
import helios.ecs.entity.EntityAccessSet;
import helios.ecs.entity.Query;

import helios.ecs.command.types;

import helios.engine.runtime;
import helios.engine.runtime.gameloop.types;
import helios.engine.runtime.Session;

import helios.engine.rendering;
import helios.engine.state;

using namespace helios::engine::runtime::enginestate::types;
using namespace helios::engine::rendering::shader::types;
using namespace helios::ecs::common::concepts;
using namespace helios::ecs::command;
using namespace helios::ecs::components;
using namespace helios::engine::rendering::shader::components;


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
        using EntityWorld = ecs::entity::EntityWorld;

        template<typename TRead, typename TWrite, typename TFilter = ecs::entity::Filter<ecs::entity::AnyDirty<>>>
        using Query = ecs::entity::Query<TRead, TWrite, TFilter>;

        template<typename ... TReads>
        using Read = ecs::entity::ReadSet<TReads...>;

        template<typename ... TWrites>
        using Write = ecs::entity::WriteSet<TWrites...>;

        using ShaderWarmupQuery = Query<
            Read<ShaderSourceComponent<ShaderHandle>>,
            Write<>,
            ecs::entity::Filter<ecs::entity::IsActive>
        >;

        using TextureWarmupQuery = Query<
            Read<rendering::texture::components::TextureSourceComponent<TextureHandle>>,
            Write<>,
            ecs::entity::Filter<ecs::entity::IsActive>
        >;

    public:


        using CommandBuffer = ecs::command::TypedCommandBuffer<StateCommand<EngineState>>;

        /**
         * @brief Queues `StateCommand<EngineState>` with `WarmupDoneSignal` when warmup resources are consumed.
         *
         * @param shaderWarmupQuery Query over active shader warmup sources.
         * @param textureWarmupQuery Query over active texture warmup sources.
         */
        void update(
            ShaderWarmupQuery shaderWarmupQuery,
            TextureWarmupQuery textureWarmupQuery,
            runtime::Session& session,
            CommandBuffer& cmdBuffer
        ) noexcept {

            if (shaderWarmupQuery.empty() && textureWarmupQuery.empty()) {

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