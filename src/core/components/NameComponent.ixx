module;

export module helios.engine.core.components.NameComponent;

import helios.engine.core.components.StringComponent;

export namespace helios::engine::core::components {

    struct NameComponentDomain{};

    /**
     * @brief Name Component.
     *
     * @tparam THandle
     */
    template<typename THandle>
    using NameComponent = StringComponent<NameComponentDomain, THandle>;

}