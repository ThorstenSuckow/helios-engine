/**
 * @file PrefabEntityPoolRequestComponent
 * @brief Component representing a request for a prefab component pool with the specified size.
 */
module;

#include <cstddef>

export module helios.engine.runtime.pooling.components:PrefabEntityPoolRequestComponent;

import helios.engine.runtime.pooling.types;

import helios.engine.core.components;


export namespace helios::engine::runtime::pooling::components {


    struct PrefabRequestDomain{};

    /**
     * @brief Component representing a request for a prefab component pool with the specified size.
     *
     * @details Owning entity should provide an EntityPoolKeyComponent to make sure pool is properly identified
     * for prefab.
     */
    template<typename TOwnerHandle>
    using PrefabEntityPoolRequestComponent = core::components::NumericValueComponent<PrefabRequestDomain, std::size_t, TOwnerHandle>;

};

