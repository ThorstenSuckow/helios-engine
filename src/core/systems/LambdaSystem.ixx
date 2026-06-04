/**
 * @file LambdaSystem.ixx
 * @brief Small ECS system wrapper around an update lambda.
 */
module;

#include <memory>
#include <functional>

export module helios.engine.core.systems.LambdaSystem;

import helios.engine.runtime.world.tags.SystemRole;
import helios.engine.runtime.world.UpdateContext;

import helios.ecs.components;

using namespace helios::ecs::components;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::world::tags;

export namespace helios::engine::core::systems {

    /**
     * @brief ECS system that executes a stored lambda on each update.
     * @tparam THandle Handle type used by the surrounding system graph.
     */
    template <typename THandle>
    class LambdaSystem {

        using FunctionType = std::function<void(UpdateContext&)>;
        FunctionType lambda_;

    public:

        using EngineRoleTag = SystemRole;

        /**
         * @brief Creates the system from an update callback.
         * @param lambda Callback executed in `update`.
         */
        explicit LambdaSystem(FunctionType lambda) : lambda_(std::move(lambda)) {}


        /**
         * @brief Executes the stored callback for the current update step.
         * @param updateContext Runtime context of the engine update.
         */
        void update(UpdateContext& updateContext) noexcept {
            lambda_(updateContext);
        }

    };


}