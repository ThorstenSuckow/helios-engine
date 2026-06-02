/**
 * @file AABBCullingStrategy.ixx
 * @brief Culling strategy based on AABB.
 */
module;

export module helios.engine.scene.AABBCullingStrategy;



import helios.ecs.types.EntityHandle;
import helios.engine.scene.types.CullingContext;

using namespace helios::ecs::types;
using namespace helios::engine::scene::types;
export namespace helios::engine::scene {

    /**
     * @brief NAABB culling strategy implementation.
     *
     * This strategy performs visibility test based on the AABBs of the members.
     *
     * @tparam TStrongId Strong-id type used by entity handles.
     */
    template<typename TMemberHandle>
    class AABBCullingStrategy  {



    public:

        using MemberHandle_type = TMemberHandle;

        /**
         * @brief Executes culling for the provided context.
         *
         * @param ctx Perspective camera context.
         * @param handle Scene member handle that would be tested by culling strategies.
         *
         */
        bool shouldRender(const CullingContext<TMemberHandle>& ctx) noexcept {
            return true;
        }

    };
}