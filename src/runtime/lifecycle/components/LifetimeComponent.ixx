/**
 * @file LifetimeComponent.ixx
 * @brief
 */
module;


export module helios.engine.runtime.lifecycle.components:LifetimeComponent;

import helios.engine.core.components;

export namespace helios::engine::runtime::lifecycle::components {

    struct LifetimeComponentDomain{};

    template<typename TOwnerHandle>
    using LifetimeComponent = core::components::NumericValueComponent<LifetimeComponentDomain, float, TOwnerHandle>;

}