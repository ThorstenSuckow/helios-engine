/**
 * @file CanInitializeRenderBackend.ixx
 * @brief Concept requiring render backend initialization lifecycle methods.
 */
module;

#include <concepts>

export module helios.engine.rendering.common.concepts.CanInitializeRenderBackend;


import helios.engine.scene.types.SceneMemberRenderContext;

using namespace helios::engine::scene::types;
export namespace helios::engine::rendering::common::concepts {

    /**
     * @brief Constrains types that support backend initialization state.
     *
     * @tparam T Backend candidate type.
     *
     * @details Requires `bool init()` and `bool isInitialized()`.
     */
    template<typename T>
    concept CanInitializeRenderBackend = requires(T& t)
    {
        {t.init()} -> std::same_as<bool>;
        {t.isInitialized()}->std::same_as<bool>;
    };




}