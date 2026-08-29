/**
 * @file MaterialOverrideComponent.ixx
 * @brief Stores an explicit material handle override.
 */
module;

export module helios.engine.rendering.common.components.MaterialOverrideComponent;

import helios.ecs.component.components;

export namespace helios::engine::rendering::common::components {

    struct MaterialOverrideComponentDomainTag;
    template<typename TOwnerHandle, typename TMaterialHandle>
    using MaterialOverrideComponent = ecs::components::BindingComponent<TOwnerHandle, TMaterialHandle,MaterialOverrideComponentDomainTag>;

};