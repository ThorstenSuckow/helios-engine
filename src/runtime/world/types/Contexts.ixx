module;

export module helios.engine.runtime.world.types.Contexts;

import helios.engine.runtime.world.UpdateContext;
import helios.ecs;

export namespace helios::engine::runtime::world::types {

    class SystemUpdateContext {
        UpdateContext& updateContext_;

    public:
        explicit SystemUpdateContext(UpdateContext& updateContext) : updateContext_(updateContext) {};
        UpdateContext& updateContext() {
            return updateContext_;
        }
    };


    class ManagerExecutionContext {
        UpdateContext& updateContext_;
        ecs::EcsWorld& ecsWorld_;
    public:
        explicit ManagerExecutionContext(UpdateContext& updateContext, ecs::EcsWorld& ecsWorld) :
        updateContext_(updateContext), ecsWorld_(ecsWorld) {};
        UpdateContext& updateContext() {
            return updateContext_;
        }
        template<typename THandle>
        ecs::EntityManager<THandle>& entityManager() {
            return ecsWorld_.entityManager<THandle>();
        }
    };

    class DefaultInitContext {
        ecs::command::CommandHandlerRegistry& commandHandlerRegistry_;
    public:
        explicit DefaultInitContext(ecs::command::CommandHandlerRegistry& commandHandlerRegistry) : commandHandlerRegistry_(commandHandlerRegistry) {};

        ecs::command::CommandHandlerRegistry& commandHandlerRegistry() {
            return commandHandlerRegistry_;
        }
    };

    class CommandBufferFlushContext {
        ecs::command::CommandHandlerRegistry& commandHandlerRegistry_;
        ecs::manager::ManagerRegistry& managerRegistry_;
    public:
        explicit CommandBufferFlushContext(
            ecs::command::CommandHandlerRegistry& commandHandlerRegistry,
            ecs::manager::ManagerRegistry& managerRegistry)
        : commandHandlerRegistry_(commandHandlerRegistry), managerRegistry_(managerRegistry) {};

        ecs::command::CommandHandlerRegistry& commandHandlerRegistry() {
            return commandHandlerRegistry_;
        }

        ecs::manager::ManagerRegistry& managerRegistry() {
            return managerRegistry_;
        }
    };

}