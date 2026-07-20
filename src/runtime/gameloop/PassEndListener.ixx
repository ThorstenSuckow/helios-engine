/**
 * @file PassEndListener.ixx
 * @brief Interface for listeners notified when a pass ends.
 */
module;


export module helios.engine.runtime.gameloop:PassEndListener;

import :Pass;

import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.UpdateContext;

using namespace helios::engine::runtime::world;

export namespace helios::engine::runtime::gameloop {

    /**
     * @brief Interface for receiving notifications when a pass reaches its end.
     *
     * @see GameLoop
     * @see Phase
     * @see Pass
     * @see CommitPoint
     */
    class PassEndListener {


    public:

        /**
         * @brief Virtual destructor for proper polymorphic cleanup.
         */
        virtual ~PassEndListener() = default;

        /**
         * @brief Called when a pass reaches its end.
         *
         * @param pass The pass that ended.
         * @param gameWorld The game world for which the pass occurred.
         * @param updateContext The current frame's update context.
         *
         * @note This method is marked `noexcept` as it is called during the game loop
         *       hot path and should not throw exceptions.
         *
         * @see CommitPoint
         * @see Pass::addCommitPoint()
         */
        virtual void onPassEnd(
            Pass& pass,
            GameWorld& gameWorld,
            UpdateContext& updateContext) noexcept = 0;
    };


}