/**
 * @file AABBCullingStrategy.ixx
 * @brief Culling strategy based on AABB.
 */
module;

#include <cmath>

export module helios.engine.scene.AABBCullingStrategy;


import helios.math;

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
         * @details Executes culling based on the AABB available with the specified CullingContext.
         * The implementation follows Miettinen (Real Time Rendering, 4th, pp. 970-971) and check if the distance
         * `s` of the center of the AABB to the plane normal satisfies the following condition:
         *  If `s` plus the extent `e` of the AABB is < 0 for any plane, the AABB is `outside`.
         *  Otherwise, the represented member is rendered.
         *
         * @param ctx Perspective camera context.
         * @param handle Scene member handle that would be tested by culling strategies.
         *
         * @note The
         */
        bool shouldRender(const CullingContext<TMemberHandle>& ctx) noexcept {

            auto& planes = ctx.frustumPlanes;
            auto& bounds = ctx.bounds;

            const auto c = bounds.center();
            const auto h = bounds.extent();

            for (auto& plane : planes) {
                const auto n = plane.normal;

                const auto e = h[0] * std::abs(n[0]) + h[1] * std::abs(n[1]) + h[2] * std::abs(n[2]);

                const auto s = c.dot(n) - plane.distance;

                if (s + e < 0 ) { // OUTSIDE
                    return false;
                }
            }


            return true; // INSIDE
        }

    };
}