/**
 * @file renderModes.ixx
 * @brief Marker types that select instanced or non-instanced render paths.
 */
module;

#include <cstdint>
#include <type_traits>
#include <utility>

export module helios.engine.rendering.common.types.submissionModes;


export namespace helios::engine::rendering::common::types {

    /**
     * @brief Marker for instanced rendering mode.
     */
    struct Instanced{};

    /**
     * @brief Marker for non-instanced rendering mode.
     */
    struct NonInstanced{};

    enum class SubmissionMode : std::uint32_t {
        Instanced,
        NonInstanced
    };


}
