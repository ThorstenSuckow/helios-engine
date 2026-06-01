module;

export module helios.engine.core.components.DebugNameComponent;

import helios.engine.core.components.StringComponent;

export namespace helios::engine::core::components {

    struct DebugNameComponentDomain{};

    /**
     * @brief Debug Name Component.
     *
     * @details Debug Name Component. Implementing APIs should not rely
     * on the availability of this component w/ selected entities in release builds.
     *
     * @tparam THandle
     */
    template<typename THandle>
    using DebugNameComponent = StringComponent<DebugNameComponentDomain, THandle>;

}