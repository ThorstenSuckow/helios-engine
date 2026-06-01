module;

#include <string>

export module helios.engine.core.components.StringComponent;

import helios.math.types;

import helios.math.concepts;

using namespace helios::math;
using namespace helios::math::concepts;
export namespace helios::engine::core::components {

    /**
     * @brief Generic String Component.
     *
     * @tparam TDomainTag
     * @tparam THandle
     */
    template<typename TDomainTag, typename THandle>
    struct StringComponent  {

        std::string value{};

        StringComponent() = default;

        explicit StringComponent(std::string&& str) : value(std::move(str)){}

    };

}
