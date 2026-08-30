/**
 * @file PlatformInitSystem.ixx
 * @brief System that requests runtime platform initialization while session is not initialized.
 */
module;

export module helios.engine.platform.lifecycle.systems.PlatformInitSystem;



import helios.engine.runtime.gameloop.types;


import helios.ecs.command.types;

import helios.engine.runtime.Session;
import helios.ecs;

import helios.engine.platform.lifecycle.commands;




using namespace helios::ecs;
using namespace helios::ecs::common::concepts;
using namespace helios::engine::platform::lifecycle::commands;
export namespace helios::engine::platform::lifecycle::systems {

    /**
     * @brief Submits `PlatformInitCommand` until session/runtime initialization completed.
     */
    class PlatformInitSystem {

        using UpdateContext = engine::runtime::gameloop::types::UpdateContext;

        public:


        using CommandBuffer = ecs::command::TypedCommandBuffer<PlatformInitCommand>;

        /**
         * @brief Queues platform initialization command when required.
         *
         * @param updateContext Frame-local update context.
         */
        void update(UpdateContext& updateContext, runtime::Session& session, CommandBuffer& cmdBuffer) noexcept {

            if (!session.isInitialized()) {
                 cmdBuffer.template add<PlatformInitCommand>();
            }
        }

    };



}