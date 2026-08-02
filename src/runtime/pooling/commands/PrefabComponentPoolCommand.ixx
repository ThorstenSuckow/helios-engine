module;

#include <cstddef>

export module helios.engine.runtime.pooling.commands:PrefabComponentPoolCommand;

import helios.engine.runtime.pooling.types;

export namespace helios::engine::runtime::pooling::commands {


    /**
     * @brief Command to prefabricate a component pool for a prefab component.
     *
     * @tparam TOwnerHandle The type of the owner handle.
     */
    template<typename TOwnerHandle>
    struct PrefabComponentPoolCommand {
        /**
         * @brief The id of the pool to prefab.
         */
        types::EntityPoolId<TOwnerHandle> entityPoolId;

        /**
         * @brief The handle of the prefab to use.
         */
        TOwnerHandle prefabHandle;

        /**
         * @brief Number of copies to create from the prefab (including the prefab).
         */
        std::size_t amount;


    };


}