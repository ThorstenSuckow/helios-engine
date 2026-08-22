/**
 * @file WindowBasedShutdownSystem.ixx
 * @brief System that requests runtime shutdown when no active windows remain.
 */
module;

export module helios.engine.platform.lifecycle.systems.WindowBasedShutdownSystem;



import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.types;
import helios.engine.runtime.concepts;

import helios.ecs.system.tags;
import helios.ecs.command.types;

import helios.engine.runtime.world;
import helios.ecs;

import helios.engine.platform.window.components;
import helios.engine.platform.window.concepts;

import helios.engine.platform.lifecycle.commands;
import helios.engine.platform.environment.components;
import helios.engine.platform.environment.concepts;


using namespace helios::engine::runtime::world;
using namespace helios::ecs;
using namespace helios::ecs::common::concepts;
using namespace helios::engine::platform::environment::components;
using namespace helios::engine::platform::window::components;
using namespace helios::engine::platform::window::concepts;
using namespace helios::engine::platform::environment::concepts;
using namespace helios::engine::platform::lifecycle::commands;
using namespace helios::ecs::components;

export namespace helios::engine::platform::lifecycle::systems {

    /**
     * @brief Queues `ShutdownCommand` when no active window entities are left.
     *
     * @tparam THandle Window handle type.
     */
    template<typename THandle>
    requires IsWindowHandle<THandle>
    class WindowBasedShutdownSystem {

    public:

        /**
         * @brief Engine role marker used by runtime registries.
         */
        using EcsRoleTag = ecs::system::tags::TypedSystemRole;
        using CommandBuffer = ecs::command::TypedCommandBuffer<ShutdownCommand>;

        /**
         * @brief Checks window activity and queues shutdown when the set is empty.
         *
         * @param updateContext Frame-local update context.
         * @param cmdBuffer Command buffer for submitting shutdown commands.
         */
        void update(UpdateContext& updateContext, CommandBuffer& cmdBuffer) noexcept {

            if (updateContext.template view<THandle, WindowComponent<THandle>>().withActive().empty()) {
               cmdBuffer.template add<ShutdownCommand>();
            }

        }

    };



}