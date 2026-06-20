/**
 * @file CallableSystem.ixx
 * @brief Small ECS system wrapper around an update lambda.
 */
module;

#include <memory>
#include <functional>
#include <type_traits>
#include <utility>

export module helios.engine.core.systems.CallableSystem;

import helios.engine.runtime.world.tags.SystemRole;
import helios.engine.runtime.world.UpdateContext;

import helios.ecs.components;

using namespace helios::ecs::components;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::world::tags;

export namespace helios::engine::core::systems {

    /**
     * @brief ECS system that executes a stored lambda on each update.
     *
     * @tparam THandle Handle type used by the surrounding system graph.
     * @tparam TFunc lambda function type
     */
    template <typename THandle, typename TFunc>
    class CallableSystem {

        TFunc lambda_;

    public:

        using EngineRoleTag = CallableSystemRole;

        /**
         * @brief Creates the system from an update callback.
         * @param lambda Callback executed in `update`.
         */
        explicit CallableSystem(TFunc lambda) : lambda_(std::move(lambda)) {}


        /**
         * @brief Executes the stored callback for the current update step.
         * @param updateContext Runtime context of the engine update.
         */
        void update(UpdateContext& updateContext) noexcept {
            std::invoke(lambda_, updateContext);
        }

    };

    /**
     * @brief Helper function for creating a CallableSystem with deduced function type.
     *
     * @tparam THandle Handle to distinguish the system's domain (e.g., entity handle type).
     * @tparam TFunc Callable type deduced from the function object.
     *
     * @param func Callable executed by the created lambda system.
     *
     * @return A CallableSystem specialized for the given handle/tag type and callable.
     */
    template<typename THandle, typename TFunc>
    [[nodiscard]] auto callableSystemForLambda(TFunc&& func) {
        using FuncType = std::remove_cvref_t<TFunc>;

        return CallableSystem<THandle, FuncType>(std::forward<TFunc>(func));

    };
}