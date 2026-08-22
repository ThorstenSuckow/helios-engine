/**
 * @file PlatformInitSystem.ixx
 * @brief System that requests runtime platform initialization while session is not initialized.
 */
module;

export module helios.engine.platform.lifecycle.systems.PlatformInitSystem;



import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.types;
import helios.engine.runtime.concepts;


import helios.ecs.command.types;

import helios.engine.runtime.world;
import helios.ecs;

import helios.engine.platform.lifecycle.commands;



using namespace helios::engine::runtime::world;
using namespace helios::ecs;
using namespace helios::ecs::common::concepts;
using namespace helios::engine::platform::lifecycle::commands;
export namespace helios::engine::platform::lifecycle::systems {

    /**
     * @brief Submits `PlatformInitCommand` until session/runtime initialization completed.
     */
    class PlatformInitSystem {

        public:


        using CommandBuffer = ecs::command::TypedCommandBuffer<PlatformInitCommand>;

        /**
         * @brief Queues platform initialization command when required.
         *
         * @param updateContext Frame-local update context.
         */
        void update(UpdateContext& updateContext, CommandBuffer& cmdBuffer) noexcept {

            if (!updateContext.session().isInitialized()) {
                 cmdBuffer.template add<PlatformInitCommand>();
            }
        }

    };



}