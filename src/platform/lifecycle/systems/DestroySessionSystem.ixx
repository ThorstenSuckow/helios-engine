/**
 * @file DestroySessionSystem.ixx
 * @brief System that transitions the current session into the destroyed state.
 */
module;

export module helios.engine.platform.lifecycle.systems.DestroySessionSystem;


import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.types;
import helios.engine.runtime.concepts;
import helios.engine.runtime.world.Session;
import helios.ecs.system.tags;

using namespace helios::engine::runtime::world;

export namespace helios::engine::platform::lifecycle::systems {

    /**
     * @brief System that marks the active session as destroyed.
     *
     * This system is typically executed during shutdown handling to stop further
     * world processing in subsequent frames.
     */
    class DestroySessionSystem {

    public:

        /**
         * @brief Engine role marker used by runtime system registries.
         */
        using EcsRoleTag = ecs::system::tags::TypedSystemRole;

        /**
         * @brief Destroys the active session in the current update context.
         *
         * @param updateCtx Frame-local update context.
         * @return true if the session was destroyed, false otherwise.
         */
        template<typename TUpdateContextType>
        requires runtime::concepts::ProvidesUpdateContext<TUpdateContextType, UpdateContext>
        void update(TUpdateContextType& updateCtx) noexcept {
            /**
             * @todo should be command
             */
            auto& updateContext = updateCtx.updateContext();
            updateContext.session().destroy();
        }
    };

}