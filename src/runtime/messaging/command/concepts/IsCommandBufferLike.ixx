/**
 * @file IsCommandBufferLike.ixx
 * @brief Concept constraining types usable as command buffers.
 */
module;

#include <concepts>

export module helios.engine.runtime.messaging.command.concepts.IsCommandBufferLike;

import helios.engine.runtime.concepts.HasTag;
import helios.engine.runtime.concepts.HasClear;

import helios.engine.runtime.messaging.command.CommandHandlerRegistry;
import helios.engine.runtime.world.ManagerRegistry;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.runtime.messaging.command.tags.CommandBufferRole;


using namespace helios::engine::runtime::messaging::command::tags;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::concepts;
export namespace helios::engine::runtime::messaging::command::concepts {

    /**
     * @brief Constrains types that can serve as concrete command buffers.
     *
     * @tparam T The type to constrain.
     *
     * @see CommandBuffer
     * @see CommandBufferRole
     * @see HasClear
     */
    template<class T>
    concept IsCommandBufferLike = requires(T& t, UpdateContext& updateContext, CommandHandlerRegistry& reg, ManagerRegistry& tm) {
        {t.flush(updateContext) } -> std::same_as<void>;
        {t.init(reg, tm)}->std::same_as<void>;
    } && HasClear<T> && HasTag<T, CommandBufferRole>;
}
