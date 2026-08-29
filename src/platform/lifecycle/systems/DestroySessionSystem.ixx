/**
 * @file DestroySessionSystem.ixx
 * @brief System that transitions the current session into the destroyed state.
 */
module;

export module helios.engine.platform.lifecycle.systems.DestroySessionSystem;


import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.concepts;
import helios.engine.runtime.common.Session;


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
         * @brief Destroys the active session in the current update context.
         *
         * @param updateContext Frame-local update context.
         * @return true if the session was destroyed, false otherwise.
         */
        void update(runtime::common::Session& session) noexcept {
            /**
             * @todo should be command
             */
            session.destroy();
        }
    };

}