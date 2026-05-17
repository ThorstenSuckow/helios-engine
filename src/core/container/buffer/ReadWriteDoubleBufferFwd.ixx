/**
 * @file ReadWriteDoubleBufferFwd.ixx
 * @brief RFwd declaration for ReadWriteDoubleBufferFwd.
 */
module;

#include <span>
#include <vector>
#include <cstddef>

export module helios.engine.core.container.buffer:ReadWriteDoubleBufferFwd;

export namespace helios::engine::core::container::buffer {
    template <typename T>
    class ReadWriteDoubleBuffer;
}