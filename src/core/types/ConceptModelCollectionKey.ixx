/**
 * @file ConceptModelCollectionKey.ixx
 * @brief key struct for identifying typeId and insertion index of data in a ConceptModelCollection.
 */
module;

#include <cstddef>

export module helios.engine.core.types:ConceptModelCollectionKey;

export namespace helios::engine::core::types {

    /**
     * @brief Simple key struct for ConceptModelCollections, stores the typeId and the insertion index.
     * @tparam TTypeIdProvider
     */
    template<typename TTypeIdProvider>
    struct ConceptModelCollectionKey {

        TTypeIdProvider typeId;

        std::size_t index;

    };


}
