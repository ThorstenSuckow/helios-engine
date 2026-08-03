module;

#include <cstddef>

export module helios.engine.runtime.pooling.components:EntityPoolPrefabComponent;

import helios.engine.runtime.pooling.types;

export namespace helios::engine::runtime::pooling::components {

    /**
     * @brief Marker component for an entity that should be used as a prefab exemplar for copying (w/o this component).
     *
     * @tparam TOwnerHandle
     */
    template<typename TOwnerHandle>
    struct EntityPoolPrefabComponent {
        /**
         * @brief The pool key that should be used for the pool that is created.
         */
        types::EntityPoolKey<TOwnerHandle> entityPoolKey;

        /**
         * @brief Number of copies of the owning entities, including the owning entity.
         */
        std::size_t amount;
    };


};

