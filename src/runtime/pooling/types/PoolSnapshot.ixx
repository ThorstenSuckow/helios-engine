module;

#include <cstddef>

export module helios.engine.runtime.pooling.types:PoolSnapshot;


export namespace helios::engine::runtime::pooling::types {

    /**
     * @brief Struct providing snapshot information for a Pool.
     */
    struct PoolSnapshot {

        std::size_t activeCount;

        std::size_t inactiveCount;

    };

}