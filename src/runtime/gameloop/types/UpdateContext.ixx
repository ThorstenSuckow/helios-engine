/**
 * @file UpdateContext.ixx
 * @brief Per-frame context passed to systems during game loop updates.
 */
module;

#include <span>
#include <cassert>
#include <utility>

export module helios.engine.runtime.gameloop.types:UpdateContext;

import helios.engine.input.InputSnapshot;

import helios.ecs.common.types;


export namespace helios::engine::runtime::gameloop::types {


    /**
     * @brief Per-frame context passed to systems during game loop updates.
     */
    class UpdateContext {

    private:
        /**
         * @brief Time elapsed since the last frame, in seconds.
         */
        float deltaTime_ = 0.0f;

        /**
        * @brief Time elapsed since the first frame, in seconds.
         */
        float totalTime_ = 0.0f;

        /**
        * @brief The current frame count, since the GameLoop started.
         */
        std::size_t frameCount_ = 0;

        /**
         * @brief Immutable snapshot of input state for the current frame.
         */
        const helios::engine::input::InputSnapshot& inputSnapshot_;

    public:


        /**
         * @brief Constructs an UpdateContext with all per-frame dependencies.
         *
         * @param deltaTime Time since last frame in seconds.
         * @param totalTime Accumulated time in seconds.
         * @param frameCount Accumulated frames.
         * @param inputSnapshot Immutable frame input snapshot.
         */
        UpdateContext(
            const float deltaTime,
            const float totalTime,
            const std::size_t frameCount,
            const helios::engine::input::InputSnapshot& inputSnapshot
        ) :
        deltaTime_(deltaTime),
        totalTime_(totalTime),
        frameCount_(frameCount),
        inputSnapshot_(inputSnapshot)
        {

        }



        /**
         * @brief Returns the time elapsed since the last frame, in seconds.
         *
         * @return Delta time in seconds.
         */
        [[nodiscard]] float deltaTime() const noexcept {
            return deltaTime_;
        }

        /**
         * @brief Returns the time elapsed since the first frame, in seconds.
         *
         * @return Total time in seconds.
         */
        [[nodiscard]] float totalTime() const noexcept {
            return totalTime_;
        }

        /**
         * @brief Returns the current frame count.
         *
         * @return Current frame count.
         */
        [[nodiscard]] std::size_t frameCount() const noexcept {
            return frameCount_;
        }

        /**
         * @brief Returns the immutable input snapshot for this frame.
         *
         * @return Const ref to the current InputSnapshot.
         */
        [[nodiscard]] const helios::engine::input::InputSnapshot& inputSnapshot() const noexcept {
            return inputSnapshot_;
        }


    };
}

