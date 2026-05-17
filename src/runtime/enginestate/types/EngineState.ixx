/**
 * @file EngineState.ixx
 * @brief Game state enumeration and bitmask operations.
 */
module;

#include <cstdint>
#include <cstddef>

export module helios.engine.runtime.enginestate.types.EngineState;



export namespace helios::engine::runtime::enginestate::types {

    /**
     * @brief Underlying type for EngineState bitmask values.
     */
    using EngineStateType = uint16_t;

    constexpr size_t EngineStateTypeSize = 16;

    /**
     * @brief Enumeration of possible game states.
     *
     * States can be combined using bitwise operators for masking.
     */
    enum class EngineState : EngineStateType {

        Undefined = 0,

        Booting = 1 << 0,

        Booted = 1 << 1,

        Warmup = 1 << 2,

        Running = 1 << 3,

        Shutdown = 1 << 4,


        /**
         * @brief Bitmask representing all base/ single-bit state flags.
         */
        Any = Booting | Booted | Running | Shutdown | Warmup,

        /**
         * @brief Bitmask representing any running state, i.e. any state except Booting, Booted, Warmup and Shutdown.
         */
        Live = Any & ~(Booting | Booted | Warmup | Shutdown),


    };

    /**
     * @brief Bitwise OR operator for combining game states.
     */
    [[nodiscard]] constexpr EngineState operator|(const EngineState lhs, const EngineState rhs) noexcept {
        return static_cast<EngineState>(static_cast<EngineStateType>(lhs) | static_cast<EngineStateType>(rhs));
    }

    /**
     * @brief Bitwise AND operator for masking game states.
     */
    [[nodiscard]] constexpr EngineState operator&(const EngineState lhs, const EngineState rhs) noexcept {
        return static_cast<EngineState>(static_cast<EngineStateType>(lhs) & static_cast<EngineStateType>(rhs));
    }

    /**
     * @brief Bitwise NOT operator for inverting game state bits.
     */
    [[nodiscard]] constexpr EngineState operator~(const EngineState lhs) noexcept {
        return static_cast<EngineState>((~static_cast<EngineStateType>(lhs)) & static_cast<EngineStateType>(EngineState::Any));
    }


    /**
     * @brief Checks if a flag is set in a state mask.
     *
     * @param mask The state mask to check.
     * @param flag The flag to check for.
     * @return True if the flag is set in the mask.
     */
    [[nodiscard]] constexpr bool hasFlag(const EngineState mask, const EngineState flag) noexcept {
        return (mask & flag) == flag;
    }

}

