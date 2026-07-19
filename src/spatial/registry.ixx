/**
 * @file registry.ixx
 * @brief Component registration for spatial transform module.
 */
module;

export module helios.engine.spatial.registry;

import helios.ecs.ComponentReflector;
import helios.engine.spatial.components;
import helios.engine.core.types.ComponentTypeTags;

using namespace helios::engine::core::types;
export namespace helios::engine::spatial {

    /**
     * @brief Registers all spatial transform components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        using R = helios::ecs::ComponentReflector<TEntityManager>;

        R::template registerType<components::TransformComponent<typename TEntityManager::Handle_type, Local>>();
        R::template registerType<components::TransformComponent<typename TEntityManager::Handle_type, World>>();

    }

}

