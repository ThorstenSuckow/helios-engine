/**
 * @file SceneMemberVisibilityRegistry.ixx
 * @brief Registry for visible and culled scene members per viewport.
 */
module;

#include "helios-engine-config.h"
#include <span>

export module helios.engine.scene.SceneMemberVisibilityRegistry;

import helios.engine.core.container.HandleManyToManyMap;

import helios.engine.scene.types;
import helios.engine.rendering.viewport.types;

using namespace helios::engine::rendering::viewport::types;
using namespace helios::engine::scene::types;
using namespace helios::engine::core::container;
export namespace helios::engine::scene {


    /**
     * @brief Stores visible and culled scene members grouped by viewport.
     * @tparam TMemberHandle Handle type of tracked scene members.
     */
    template<typename TMemberHandle>
    class SceneMemberVisibilityRegistry {

        HandleManyToManyMap<ViewportHandle, TMemberHandle> visibleMembers_;
        HandleManyToManyMap<ViewportHandle, TMemberHandle> culledMembers_;

    public:
        /**
         * @brief Creates an empty registry with default viewport capacity.
         */
        SceneMemberVisibilityRegistry() {
            visibleMembers_.reserve(DEFAULT_VIEWPORT_POOL_CAPACITY);
            culledMembers_.reserve(DEFAULT_VIEWPORT_POOL_CAPACITY);
        }

        /**
         * @brief Adds a member to the culled list of a viewport.
         * @param viewportHandle Viewport receiving the culled member.
         * @param memberHandle Scene member to register as culled.
         * @return `true` after the association was appended.
         */
        [[nodiscard]] bool addCulledMember(ViewportHandle viewportHandle, TMemberHandle memberHandle) {
            return culledMembers_.bind(viewportHandle, memberHandle);
        }

        /**
         * @brief Adds a member to the visible list of a viewport.
         * @param viewportHandle Viewport receiving the visible member.
         * @param memberHandle Scene member to register as visible.
         * @return `true` after the association was appended.
         */
        [[nodiscard]] bool addVisibleMember(ViewportHandle viewportHandle, TMemberHandle memberHandle) {
            return visibleMembers_.bind(viewportHandle, memberHandle);
        }

        /**
         * @brief Returns culled members for a viewport.
         * @param viewportHandle Viewport to query.
         * @return Read-only span of culled members, or an empty span.
         */
        [[nodiscard]] std::span<const TMemberHandle> culledMembers(ViewportHandle viewportHandle) const {
            return culledMembers_.values(viewportHandle);
        }

        /**
         * @brief Returns visible members for a viewport.
         * @param viewportHandle Viewport to query.
         * @return Read-only span of visible members, or an empty span.
         */
        [[nodiscard]] std::span<const TMemberHandle> visibleMembers(ViewportHandle viewportHandle) const {
            return visibleMembers_.values(viewportHandle);
        }


        /**
         * @brief Clears visible and culled members for all viewports.
         */
        void clear () {
            visibleMembers_.clear();
            culledMembers_.clear();

        }
    };


}