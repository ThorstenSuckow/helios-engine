module;

#include <cstdint>

export module helios.engine.runtime.gameloop.types:PhaseType;


export namespace helios::engine::runtime::gameloop::types {

    enum class PhaseType : std::uint8_t{
        Pre,
        Main,
        Post
    };

} // namespace helios::engine::runtime::gameloop::types