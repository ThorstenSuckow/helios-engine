module;

#include <cstddef>

export module helios.engine.runtime.pooling.components:PrefabRequestComponent;

import helios.engine.runtime.pooling.types;

import helios.engine.core.components;


export namespace helios::engine::runtime::pooling::components {


    struct PrefabRequestDomain{};

    template<typename TOwnerHandle>
    using PrefabRequestComponent = core::components::NumericValueComponent<PrefabRequestDomain, std::size_t, TOwnerHandle>;

};

