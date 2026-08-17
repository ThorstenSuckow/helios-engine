/**
 * @file EntityPool.ixx
 * @brief Object pool for efficient Entity management and recycling.
 *
 * Provides a memory-efficient pooling mechanism for frequently spawned and despawned
 * Entities, such as projectiles, particles, or enemies. Pre-allocates objects at
 * construction time and reuses them to avoid runtime allocation overhead.
 */
module;

#include <memory>
#include <unordered_map>
#include <vector>
#include <cassert>
#include <limits>
#include <span>
#include <cstddef>


export module helios.engine.runtime.pooling.EntityPool;

import helios.ecs.common.types;

import helios.core.log;

#define HELIOS_LOG_SCOPE "helios::engine::runtime::pooling::EntityPool"
export namespace helios::engine::runtime::pooling {


    /**
     * @brief Object pool for efficient Entity lifecycle management.
     *
     * @details EntityPool manages a fixed-size collection of Entity identifiers,
     * tracking which objects are currently active (in-use) and which are inactive (available).
     * This pattern eliminates runtime allocation overhead for frequently spawned entities
     * like projectiles, particles, or enemies.
     *
     * The pool uses O(1) operations for both acquire and release:
     * - **acquire**: Pops from the inactive list and adds to active tracking
     * - **release**: Swap-and-pop removal from active list, push to inactive
     *
     * Entities themselves are owned by GameWorld; this pool only tracks their EntityHandles.
     *
     * @todo Prevent duplicate EntityHandles from being added to the pool.
     */
    template <typename THandle>
    class EntityPool {

        static inline auto& logger_ = helios::core::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);

        /**
         * @brief Maps active Entity EntityIds to their index in activeEntities_.
         *
         * Enables O(1) lookup for release operations.
         */
        std::vector<size_t> activeIndex_;

        /**
         * @brief Tracks version numbers for active EntityHandles.
         *
         * Used to validate that a release operation targets the correct entity version.
         */
        std::vector<size_t> versionIndex_;

        /**
         * @brief List of EntityHandles for currently active (in-use) Entities.
         */
        std::vector<THandle> activeEntities_;

        /**
         * @brief List of EntityHandles for currently inactive (available) Entities.
         */
        std::vector<THandle> inactiveEntities_;

        /**
         * @brief The maximum number of objects this pool manages.
         */
        size_t poolSize_ = 0;

        /**
         * @brief Minimum EntityId in the pool (used for sparse array offset).
         */
        helios::ecs::common::types::EntityId minEntityId_ = std::numeric_limits<helios::ecs::common::types::EntityId>::max();

        /**
         * @brief Maximum EntityId in the pool (used for sparse array sizing).
         */
        helios::ecs::common::types::EntityId maxEntityId_ = std::numeric_limits<helios::ecs::common::types::EntityId>::lowest();

        /**
         * @brief Offset for sparse array indexing (equals minEntityId_ after lock).
         */
        size_t delta_ = 0;

        /**
         * @brief True if the pool is locked and ready for acquire/release operations.
         */
        bool locked_ = false;

    public:
        /**
         * @brief Move only.
         */
        EntityPool(const EntityPool<THandle>&) = delete;
        EntityPool& operator=(const EntityPool<THandle>&) = delete;
        EntityPool(EntityPool<THandle>&&) noexcept = default;
        EntityPool& operator=(EntityPool<THandle>&&) noexcept = default;

        /**
         * @brief default constructor for the EntityPool.
         *
         * @details Use setPoolSize() in a separate call.
         */
        EntityPool() = default;

        /**
         * @brief Constructs an `EntityPool` with the specified capacity.
         *
         * @details Pre-allocates internal storage for the given pool size.
         * The pool starts empty; use `addInactive()` or a factory to populate it.
         *
         * @param poolSize The maximum number of Entities this pool can manage.
         */
        explicit EntityPool(const std::size_t poolSize) {
            setPoolSize(poolSize);
        }

        /**
         * @brief Sets the pool size.
         *
         * @details Sets pool size and reserves vector capacities according to it. Must be done
         * before pool is locked.
         *
         * @param poolSize The maximum number of Entities this pool can manage.
         *
         * @return True on success, otherwise false.
         */
        bool setPoolSize(const std::size_t poolSize) {

            if (locked_) {
                logger_.error("Cannot reserve pool size after locking");
                assert(false && "Cannot reserve pool size after locking");
                return false;
            }

            poolSize_ = poolSize;
            activeEntities_.reserve(poolSize);
            inactiveEntities_.reserve(poolSize);
            return true;
        }

        /**
         * @brief Returns the maximum capacity of this pool.
         *
         * @return Pool size specified at construction.
         */
        [[nodiscard]] size_t size() const noexcept {
            return poolSize_;
        }

        /**
         * @brief Acquires an inactive Entity from the pool.
         *
         * @details Removes an `EntityHandle` from the inactive list and adds it to the active
         * tracking structures. The caller is responsible for activating the actual
         * Entity in the GameWorld.
         *
         * @param[out] entityHandle  Receives the handle of the acquired Entity on success.
         *
         * @return `true` if an object was acquired, `false` if the pool is exhausted.
         */
        [[nodiscard]] bool acquire(THandle& entityHandle) {

            if (!locked_) {
                logger_.error("Pool must be locked before acquiring objects");
                assert(false && "Pool must be locked before acquiring objects");
                return false;
            }

            if (inactiveEntities_.empty()) {
                return false;
            }

            entityHandle = inactiveEntities_.back();
            inactiveEntities_.pop_back();


            auto idx = entityHandle.entityId() - delta_;

            if (activeIndex_.size() <= idx) {
                activeIndex_.resize(idx + 1, helios::ecs::common::types::EntityTombstone);
                versionIndex_.resize(idx + 1, helios::ecs::common::types::EntityTombstone);
            }

            activeIndex_[idx] = activeEntities_.size();
            versionIndex_[idx] = entityHandle.versionId();

            activeEntities_.push_back(entityHandle);

            return true;
        }

        /**
         * @brief Returns whether the pool is locked and ready for acquire/release operations.
         *
         * @return `true` if the pool is locked.
         */
        [[nodiscard]] bool isLocked() const noexcept {
            return locked_;
        }

        /**
         * @brief Locks the pool, enabling acquire/release operations.
         *
         * @details Computes the sparse-array bounds from the min/max EntityIds registered via
         * `addInactive()`. After locking, no further `addInactive()` calls are permitted.
         *
         * @return `true` on success, `false` if the pool is empty or the computed range is invalid.
         */
        [[nodiscard]] bool lock() noexcept {

            delta_ = minEntityId_;

            const auto size = maxEntityId_ - delta_ + 1;

            if (activeEntities_.empty() && inactiveEntities_.empty()) {
                logger_.warn("Pool has no entities, cannot lock.");
                assert(false && "Pool has no entities, cannot lock.");
                return false;
            }

            if (size > 1000000) {
                assert(false && "size is unreasonably large.");
                logger_.warn("Pool size is too large.");
                return false;
            }

            try {
                activeIndex_.resize(size, helios::ecs::common::types::EntityTombstone);
                versionIndex_.resize(size, helios::ecs::common::types::EntityTombstone);
            } catch (std::exception& e) {
                logger_.error("Could not lock pool, resiszing failed.");
                assert(false && "Resizing failed.");
                return false;
            }

            locked_ = true;

            return true;
        }

        /**
         * @brief Registers an `EntityHandle` as inactive without acquiring it.
         *
         * @details Used during pool initialization to populate the pool with pre-created Entities.
         * Fails if the pool is already locked or at capacity.
         *
         * @param entityHandle  Handle of the Entity to register as inactive.
         *
         * @return `true` if added successfully, `false` if the pool is locked or full.
         */
        bool addInactive(const THandle entityHandle) {

            if (locked_) {
                logger_.warn("Cannot add inactive entity, pool is locked.");
                assert(false && "Pool is locked");
                return false;
            }

            if (!entityHandle.isValid()) {
                logger_.warn("Cannot add inactive entity, invalid entityHandle.");
                assert(false && "Unexpected invalid entityHandle");
                return false;
            }

            const size_t used = (activeCount() + inactiveCount());

            minEntityId_ = std::min(minEntityId_, entityHandle.entityId());
            maxEntityId_ = std::max(maxEntityId_, entityHandle.entityId());

            if (used < size()) {
                inactiveEntities_.push_back(entityHandle);
                return true;
            }

            return false;
        }

        /**
         * @brief Releases an Entity back to the pool.
         *
         * @details Validates the handle against the active tracking structures, then
         * removes it via swap-and-pop (O(1)) and pushes it onto the inactive list
         * for future acquisition.
         *
         * @param entityHandle  Handle of the active Entity to release.
         *
         * @return `true` if released successfully, `false` if the handle was not tracked as active.
         */
        bool release(const THandle entityHandle) {

            if (!locked_) {
                logger_.warn("Cannot release entity, pool is not locked.");
                assert(false && "Cannot release entity, pool is not locked.");
                return false;
            }

            assert(entityHandle.isValid() && "Unexpected invalid entityHandle");

            assert(entityHandle.entityId() >= delta_  && "Unexpected entityHandle");

            const auto sparseIdx = entityHandle.entityId() - delta_;

            assert(sparseIdx < activeIndex_.size() && "Unexpected sparse index");

            const auto denseIndex = activeIndex_[sparseIdx];
            if (denseIndex == helios::ecs::common::types::EntityTombstone) {
                return false;
            }

            assert(versionIndex_[sparseIdx] == entityHandle.versionId() && "Version mismatch");

            auto lastEntityHandle = activeEntities_.back();

            if (denseIndex != activeEntities_.size() - 1) {
                // swap the last entityHandle in activeEntities with the
                // entityHandle to remove, effectively overwriting entityHandle
                // to release with a currently active entityHandle
                activeEntities_[denseIndex] = lastEntityHandle;
                activeIndex_[lastEntityHandle.entityId() - delta_] = denseIndex;
                versionIndex_[lastEntityHandle.entityId() - delta_] = lastEntityHandle.versionId();
            }


            // the swap operation has create a duplicate entry,
            // remove the one at the tail
            activeEntities_.pop_back();

            // clear the queried entityHandle from active index and update
            // inactiveEntities
            activeIndex_[sparseIdx] = helios::ecs::common::types::EntityTombstone;
            versionIndex_[sparseIdx] = helios::ecs::common::types::EntityTombstone;

            inactiveEntities_.push_back(entityHandle);

            return true;
        }

        /**
         * @brief Releases and permanently removes an Entity from the pool.
         *
         * @details Unlike `release()`, the handle is **not** returned to the inactive list.
         * Use this when a pooled Entity is being destroyed rather than recycled.
         *
         * @param entityHandle  Handle of the active Entity to remove.
         *
         * @return `true` if removed successfully, `false` if the handle was not tracked as active.
         */
        bool releaseAndRemove(const THandle entityHandle) {

            if (!locked_) {
                logger_.warn("Cannot release and remove entity, pool is not locked.");
                assert(false && "Cannot release and remove entity, pool is not locked.");
                return false;
            }

            assert(entityHandle.isValid() && "Unexpected invalid entityHandle");

            const auto sparseIdx = entityHandle.entityId() - delta_;

            assert(sparseIdx < activeIndex_.size() && "Unexpected sparse index");

            const auto denseIndex = activeIndex_[sparseIdx];

            if (denseIndex == helios::ecs::common::types::EntityTombstone) {
                return false;
            }

            assert(versionIndex_[sparseIdx] == entityHandle.versionId() && "Version mismatch");

            if (denseIndex != activeEntities_.size() - 1) {
                const auto lastEntityHandle = activeEntities_.back();
                activeIndex_[lastEntityHandle.entityId() - delta_] = denseIndex;
                versionIndex_[lastEntityHandle.entityId() - delta_] = lastEntityHandle.versionId();
                activeEntities_[denseIndex] = lastEntityHandle;
            }

            activeEntities_.pop_back();

            activeIndex_[sparseIdx] = helios::ecs::common::types::EntityTombstone;
            versionIndex_[sparseIdx] = helios::ecs::common::types::EntityTombstone;

            return true;

        }


        /**
         * @brief Returns the number of currently active Entities.
         *
         * @return Number of Entities that have been acquired and not yet released.
         */
        [[nodiscard]] size_t activeCount() const noexcept {
            return activeEntities_.size();
        }

        /**
         * @brief Returns the number of currently inactive Entities.
         *
         * @return Number of Entities available for acquisition.
         */
        [[nodiscard]] size_t inactiveCount() const noexcept {
            return inactiveEntities_.size();
        }

        /**
         * @brief Returns a span over all inactive `EntityHandle`s.
         *
         * @return Read-only span of handles available for acquisition.
         */
        const std::span<const THandle> inactiveEntities() {
            return inactiveEntities_;
        };

        /**
         * @brief Returns a span over all active `EntityHandle`s.
         *
         * @return Read-only span of handles currently in use.
         */
        const std::span<const THandle> activeEntities() {
            return activeEntities_;
        };
    };

}
