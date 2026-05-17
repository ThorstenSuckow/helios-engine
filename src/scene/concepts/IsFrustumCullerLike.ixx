/**
 * @file IsFrustumCullerLike.ixx
 * @brief Concept that validates frustum-culling strategy types.
 */
module;

#include<concepts>

export module helios.engine.scene.concepts.IsFrustumCullerLike;

import helios.ecs.types.EntityHandle;
import helios.engine.scene.types.CullingContext;

using namespace helios::ecs::types;
using namespace helios::engine::scene::types;
export namespace helios::engine::scene::concepts {

    /**
     * @brief Constrains types that expose the culling API used by scene extraction systems.
     *
     * A valid type provides `cull(const PerspectiveCameraContext&, const EntityHandle)`
     * returning `void`.
     *
     * @tparam T Candidate culling strategy type.
     * @tparam TStrongId type for the EntityHandle's StrongId.
     */
    template <typename T, typename TMemberHandle>
    concept IsFrustumCullerLike = requires(T& t, const TMemberHandle entityHandle, const CullingContext<TMemberHandle>& cullingContext)
    {
        {t.shouldRender(cullingContext)}-> std::same_as<bool>;

    };

}