/**
 * @file EntityPoolKeyComponent.ixx
 * @brief Component associating an entity with the EntityPoolKey of its pool.
 */
module;

export module helios.engine.runtime.pooling.components:EntityPoolKeyComponent;

import helios.engine.runtime.pooling.types;

export namespace helios::engine::runtime::pooling::components {

    /**
     * @brief Component that associates an entity with the EntityPoolKey of the pool it belongs to.
     *
     */
    struct EntityPoolKeyComponent {
        /**
         * @brief The pool key identifying the target pool for this entity.
         */
        types::EntityPoolKey entityPoolKey;

    };


};

