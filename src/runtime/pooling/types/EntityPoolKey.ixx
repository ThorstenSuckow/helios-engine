/** @file EntityPoolKey.ixx
 * @brief Key alias for a ConceptModelCollectionKey<EntityPoolTypeId>.
 */
module;

export module helios.engine.runtime.pooling.types:EntityPoolKey;

import helios.core.common.container.types;
import :EntityPoolTypeId;


export namespace helios::engine::runtime::pooling::types {

    using EntityPoolKey = helios::core::common::container::types::ConceptModelCollectionKey<EntityPoolTypeId>;

};
