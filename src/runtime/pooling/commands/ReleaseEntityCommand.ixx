/**
 * @file ReleaseEntityCommand.ixx
 * @brief Command for realising and entity back to its pool.
 */
module;

#include <cstddef>

export module helios.engine.runtime.pooling.commands:ReleaseEntityCommand;

import helios.engine.runtime.pooling.types;

export namespace helios::engine::runtime::pooling::commands {


    /**
     * @brief Command to release an entity back to its pool.
     *
     * @tparam TOwnerHandle The type of the owner handle.
     */
    template<typename TOwnerHandle>
    struct ReleaseEntityCommand {

        using Handle_type = TOwnerHandle;

        /**
         * @brief The key of the pool that should be used for releasing the entity.
         */
        types::EntityPoolKey<TOwnerHandle> entityPoolKey;

        /**
         * @brief The handle of the entity to release.
         */
        TOwnerHandle entityHandle;


    };


}