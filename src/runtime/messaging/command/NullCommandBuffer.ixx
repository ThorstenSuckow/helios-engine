/**
 * @file NullCommandBuffer.ixx
 * @brief No-op command buffer implementation used as a default command-buffer type.
 */
module;

export module helios.engine.runtime.messaging.command.NullCommandBuffer;

import helios.engine.runtime.messaging.command.CommandHandlerRegistry;
import helios.engine.runtime.world.ManagerRegistry;

import helios.engine.runtime.messaging.command.tags.CommandBufferRole;
import helios.engine.runtime.world.UpdateContext;

using namespace helios::engine::runtime::messaging::command::tags;
using namespace helios::engine::runtime::world;
export namespace helios::engine::runtime::messaging::command {

    /**
     * @brief Command buffer implementation that intentionally performs no work.
     *
     * Use this type as a default template argument when command emission is optional.
     */
    class NullCommandBuffer {

    public:

        using EngineRoleTag = CommandBufferRole;

        template<class T, class... Args>
        void add(Args&&... args) {/*intentionally noop*/}

        void flush(UpdateContext& updateContext) noexcept {/*intentionally noop*/}

        void clear() noexcept {/*intentionally noop*/}

        void init(CommandHandlerRegistry& commandHandlerRegistry, ManagerRegistry& managerRegistry) noexcept {/*intentionally noop*/}

    };


}
