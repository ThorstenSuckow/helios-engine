/**
 * @file IsSceneHandle.ixx
 * @brief Concept constraining scene handle types.
 */
module;

#include <concepts>

export module helios.engine.scene.concepts.IsSceneHandle;

import helios.engine.scene.types.SceneHandle;

using namespace helios::engine::scene::types;
export namespace helios::engine::scene::concepts {

    /**
     * @brief Constrains a type to the scene handle alias.
     *
     * @tparam T Type to validate.
     */
    template<class T>
    concept IsSceneHandle = std::is_same_v<T, SceneHandle>;

}