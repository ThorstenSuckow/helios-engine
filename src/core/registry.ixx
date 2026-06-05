/**
 * @file registry.ixx
 * @brief Component registration entry points for helios.engine.core.
 */
module;

export module helios.engine.core.registry;

import helios.ecs.ComponentReflector;

import helios.engine.core.components;

export namespace helios::engine::core {

    template<typename TEntityManager>
    inline void registerComponents() {

        using R = helios::ecs::ComponentReflector<TEntityManager>;

        R::template registerType<components::ColorComponent<typename TEntityManager::Handle_type>>();
    }

}
