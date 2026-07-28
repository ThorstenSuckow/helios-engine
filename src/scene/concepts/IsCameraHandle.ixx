/**
 * @file IsCameraHandle.ixx
 * @brief Concept constraining scene handle types.
 */
module;

#include <concepts>

export module helios.engine.scene.concepts.IsCameraHandle;

import helios.engine.scene.types.CameraHandle;

using namespace helios::engine::scene::types;
export namespace helios::engine::scene::concepts {

    /**
     * @brief Constrains a type to the camera handle alias.
     *
     * @tparam T Type to validate.
     */
    template<class T>
    concept IsCameraHandle = std::is_same_v<T, CameraHandle>;

}