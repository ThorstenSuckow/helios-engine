/**
 * @file ClearComponent.ixx
 * @brief Stores clear flags for a render-related entity.
 */
module;

export module helios.engine.rendering.common.components.ClearComponent;

import helios.engine.rendering.common.types.ClearFlags;

using namespace helios::engine::rendering::common::types;
export namespace helios::engine::rendering::common::components {

    /**
     * @brief Component that stores clear flags.
     *
     * @tparam TOwnerHandle Owning entity handle type.
     */
    template<typename TOwnerHandle>
    struct ClearComponent {

        /** @brief Active clear flags for the owning entity. */
        ClearFlags flags = ClearFlags::None;

    };


}