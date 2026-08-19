/**
 * @file WarmupDoneSystem.ixx
 * @brief System that emits a game-state warmup-done transition when no active shader sources remain.
 */
module;

#include <concepts>

export module helios.engine.platform.lifecycle.systems.WarmupDoneSystem;


import helios.ecs;

import helios.engine.runtime;
import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.types;
import helios.engine.runtime.concepts;
import helios.engine.rendering;
import helios.engine.state;

using namespace helios::engine::runtime::enginestate::types;
using namespace helios::engine::rendering::shader::concepts;
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
    class WarmupDoneSystem {

    public:

        /**
         * @brief Engine role marker used by runtime registries.
         */
        using EcsRoleTag = ecs::system::tags::TypedSystemRole;

        /**
         * @brief Queues `StateCommand<EngineState>` with `WarmupDoneSignal` when warmup resources are consumed.
         *
         * @param updateCtx Frame-local update context.
         */
        template<typename TUpdateContextType, typename TCommandBuffer>
        requires runtime::concepts::ProvidesUpdateContext<TUpdateContextType, UpdateContext> &&
                 ecs::command::concepts::IsCommandBufferLike<TCommandBuffer> &&
                 (!std::is_same_v<TCommandBuffer, NullCommandBuffer>)
        bool update(TUpdateContextType& updateCtx, TCommandBuffer& cmdBuffer) noexcept {

            auto& updateContext = updateCtx.updateContext();

            if (updateContext.template view<
                ShaderHandle,
                ShaderSourceComponent<ShaderHandle>
                >().withActive().empty() &&
                updateContext.template view<
                rendering::texture::types::TextureHandle,
                rendering::texture::components::TextureSourceComponent<rendering::texture::types::TextureHandle>
                >().withActive().empty()
                ) {

                cmdBuffer.template add<StateCommand<EngineState>>(
                    StateTransitionRequest<EngineState>(
                        updateContext.session().template state<EngineState>(),
                        EngineStateTransitionId::WarmupDone
                    )
                );

            }


            return true;
        }

    };



}