/**
 * @file PerspectiveCameraComponent.ixx
 * @brief Stores perspective projection parameters for a camera entity.
 */
module;

#include <cassert>
#include <cstddef>

export module helios.engine.scene.components.PerspectiveCameraComponent;


import helios.math;

using namespace helios::math;
export namespace helios::engine::scene::components {

    /**
     * @brief Component that stores perspective camera projection parameters.
     *
     * @tparam THandle Owning entity handle type.
     */
    template<typename THandle>
    class PerspectiveCameraComponent {

        /**
         * @brief The aspect ratio of the camera (width/height).
         *
         * Defaults to 1.0.
         */
        float aspectRatio_ = 1.0f;

        /**
         * @brief The near clipping plane distance.
         *
         * Defaults to 0.1.
         */
        float zNear_ = 0.1f;

        /**
         * @brief The far clipping plane distance.
         *
         * Defaults to 1000.0.
         */
        float zFar_ = 1000.0f;

        /**
         * @brief The vertical field of view in radians.
         *
         * Defaults to 90 degrees (converted to radians).
         */
        float fovY_ = radians(90);

        size_t previousVersion_ = 0;
        size_t currentVersion_ = 0;




    public:

        explicit PerspectiveCameraComponent(const float fovY, const float aspectRatio, const float zNear = 0.1f, const float zFar = 1000.0f) {
            setPerspective(fovY, aspectRatio, zNear, zFar);
        }

        /**
         * @brief Sets the perspective projection parameters.
         *
         * @param fovY The vertical field of view in radians.
         * @param aspectRatio The aspect ratio (width/height).
         * @param zNear The near clipping plane distance.
         * @param zFar The far clipping plane distance.
         */
        void setPerspective(const float fovY, const float aspectRatio, const float zNear, const float zFar) noexcept {
            assert(zNear > 0 && "zNear must be positive");
            assert(zFar > 0 && zFar > zNear && "zFar must be positive and greater than zNear");
            fovY_ = fovY;
            aspectRatio_ = aspectRatio;
            zNear_ = zNear;
            zFar_ = zFar;
            currentVersion_++;
        }

        /**
         * @brief Sets the near clipping plane distance.
         *
         * @param zNear The new near clipping plane distance.
         */
        void setZNear(const float zNear) noexcept {
            assert(zNear > 0 && "zNear must be positive");
            zNear_ = zNear;
            currentVersion_++;
        }

        /**
         * @brief Sets the far clipping plane distance.
         *
         * @param zFar The new far clipping plane distance.
         */
        void setZFar(const float zFar) noexcept {
            assert(zFar > 0 && zFar > zNear_ && "zFar must be positive and greater than zNear");
            zFar_ = zFar;
            currentVersion_++;
        }

        /**
         * @brief Sets the vertical field of view.
         *
         * @param fovY The new vertical field of view in radians.
         */
        void setFovY(const float fovY) noexcept {
            fovY_ = fovY;
            currentVersion_++;
        }

        /**
         * @brief Sets the aspect ratio used by the camera.
         *
         * @param aspectRatio The new aspect ratio (width/height).
         */
        void setAspectRatio(const float aspectRatio) noexcept {
           aspectRatio_ = aspectRatio;
           currentVersion_++;
        }


        /**
         * @brief Gets the current vertical field of view.
         *
         * @return The vertical field of view in radians.
         */
        [[nodiscard]] float fovY() const noexcept {
            return fovY_;
        }

        /**
         * @brief Gets the current aspect ratio.
         *
         * @return The aspect ratio (width/height).
         */
        [[nodiscard]] float aspectRatio() const noexcept {
            return aspectRatio_;
        }

        /**
         * @brief Gets the current near clipping plane distance.
         *
         * @return The near clipping plane distance.
         */
        [[nodiscard]] float zNear() const noexcept {
            return zNear_;
        }

        /**
         * @brief Gets the current far clipping plane distance.
         *
         * @return The far clipping plane distance.
         */
        [[nodiscard]] float zFar() const noexcept {
            return zFar_;
        }

        /**
         * @brief Returns whether the component requires projection recomputation.
         *
         * @return `true` when values were changed and not yet consumed.
         */
        [[nodiscard]] bool hasChanges() const noexcept {
            return currentVersion_ != previousVersion_;
        }

        /**
         * @brief Clears the dirty flag after dependent systems consumed updates.
         */
        void commit() noexcept {
            previousVersion_ = currentVersion_;
        }

    };

}



