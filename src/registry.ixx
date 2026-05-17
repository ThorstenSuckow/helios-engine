/**
 * @file registry.ixx
 * @brief Component registration for all engine modules.
 */
module;

export module helios.registry;

import helios.engine.rendering.registry;
import helios.engine.spatial.registry;
import helios.engine.scene.registry;


export namespace helios {

    /**
     * @brief Registers all module components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {

        helios::engine::rendering::registerComponents<TEntityManager>();
        helios::engine::spatial::registerComponents<TEntityManager>();
        helios::engine::scene::registerComponents<TEntityManager>();

    }

}
