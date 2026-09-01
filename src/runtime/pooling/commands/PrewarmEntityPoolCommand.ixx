module;

#include <cstddef>

export module helios.engine.runtime.pooling.commands:PrewarmEntityPoolCommand;

import helios.engine.runtime.pooling.types;

export namespace helios::engine::runtime::pooling::commands {


    /**
     * @brief Command to prefabricate an entity pool for a prefab entity.
     *
     * @tparam TOwnerHandle The type of the owner handle.
     */
    template<typename TOwnerHandle>
    struct PrewarmEntityPoolCommand {

        using HandleType = TOwnerHandle;

        /**
         * @brief The key of the pool that should be used for prefabrication.
         */
        types::EntityPoolKey entityPoolKey;

        /**
         * @brief The handle that initiated the command.
         */
        TOwnerHandle ownerHandle;

        /**
         * @brief Number of copies to create from the prefab (including the prefab).
         */
        std::size_t amount;


    };


}