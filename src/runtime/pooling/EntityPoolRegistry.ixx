/**
 * @file EntityPoolRegistry.ixx
 * @brief ConceptModelInstanceCollectionRegistry for EntityPools.
 */
module;

export module helios.engine.runtime.pooling.EntityPoolRegistry;

import helios.engine.runtime.pooling.EntityPool;
import helios.engine.runtime.pooling.types;


import helios.core.common.container;


export namespace helios::engine::runtime::pooling {


    using EntityPoolRegistry = core::common::container::ConceptModelInstanceCollectionRegistry<EntityPool, types::EntityPoolTypeId>;

}
