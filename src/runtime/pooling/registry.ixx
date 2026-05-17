/**
 * @file registry.ixx
 * @brief Component registration for runtime pooling module.
 */
module;

export module helios.engine.runtime.pooling.registry;

import helios.ecs.ComponentReflector;
import helios.engine.runtime.pooling.components;

export namespace helios::engine::runtime::pooling {

    /**
     * @brief Registers all runtime pooling components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        using R = helios::ecs::ComponentReflector<TEntityManager>;

        R::template registerType<components::PrefabIdComponent<typename TEntityManager::Handle_type>>();
    }

}


