/**
 * @file EntityPool.ixx
 * @brief Object pool for efficient Entity management and recycling.
 */
module;

#include <memory>
#include <vector>
#include <cassert>

export module helios.engine.runtime.pooling.EntityPool;

import helios.ecs;

import helios.core.log;


#define HELIOS_LOG_SCOPE "helios::engine::runtime::pooling::EntityPool"
export namespace helios::engine::runtime::pooling {




    class EntityPool {

        using HandleTypeId = ecs::common::types::HandleTypeId;

        using HandleRef = ecs::common::types::HandleRef;
        using HandleSpanRef = ecs::common::types::HandleSpanRef;
        using EntityRef = ecs::EntityRef;
        using ConstEntityRef = ecs::ConstEntityRef;
        using EntityManagerRef = ecs::EntityManagerRef;

        class Concept {

        public:

            virtual ~Concept() = default;

            [[nodiscard]] virtual HandleSpanRef inactiveEntities() const noexcept = 0;


            [[nodiscard]] virtual HandleSpanRef activeEntities() const noexcept  = 0;


        };


        template<typename THandle>
        class Model final : public Concept {

            static inline auto& logger_ = helios::core::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);


            using HandleType = THandle;

            ecs::EntityManager<THandle> entityManager_;

            std::vector<size_t> activeIndex_;
            std::vector<size_t> versionIndex_;
            std::vector<THandle> activeEntities_;
            std::vector<THandle> inactiveEntities_;
            size_t poolSize_ = 0;
            helios::ecs::common::types::EntityId minEntityId_ = std::numeric_limits<helios::ecs::common::types::EntityId>::max();
            helios::ecs::common::types::EntityId maxEntityId_ = std::numeric_limits<helios::ecs::common::types::EntityId>::lowest();
            size_t delta_ = 0;
            bool locked_ = false;
            THandle prefabHandle_;


        public:

            [[nodiscard]] EntityRef prefabEditor() noexcept {
                if (!prefabHandle_.isValid()) {
                    prefabHandle_ = entityManager_.create();
                }

                return EntityRef{prefabHandle_, &entityManager_};
            }

            [[nodiscard]] ConstEntityRef prefab() const noexcept {
                return ConstEntityRef{prefabHandle_, &entityManager_};
            }

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

            bool prewarm(EntityManagerRef entityManagerRef)  {

                ecs::EntityManager<THandle&> targetEntityManager = entityManagerRef.get<THandle>();

                if (isLocked()) {
                    assert(false && "Entity pool is already locked.");
                    return false;
                }
                const size_t used  = activeCount() + inactiveCount();
                const size_t space = used < size() ? size() - used : 0;

                if (space == 0) {
                    assert(false && "No space in pool remaining");
                    return false;
                }

                for (size_t i = 0; i < space; i++) {
                    if (auto targetHandle = targetEntityManager.copyFrom(entityManager_, prefabHandle_);
                        targetHandle.isValid()) {
                        ecs::Entity<ecs::EntityManager<THandle>> targetEntity{targetHandle, &targetEntityManager};
                        targetEntity.setActive(false);
                        addInactive(targetHandle);
                    }
                }

                if (!lock()) {
                    logger_.error("Failed to lock entity pool.");
                    assert(false && "Entity pool could not be locked.");
                    return false;
                }

                return true;
            }

            [[nodiscard]] size_t size() const noexcept {
                return poolSize_;
            }


            [[nodiscard]] bool acquire(const HandleRef handleRef) {

                THandle entityHandle = handleRef.get<THandle>();

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


            [[nodiscard]] bool isLocked() const noexcept {
                return locked_;
            }


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


            bool addInactive(const HandleRef handleRef) {

                THandle entityHandle = handleRef.get<THandle>();

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


            bool release(const HandleRef handleRef) {

                THandle entityHandle = handleRef.get<THandle>();


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

            bool releaseAndRemove(const HandleRef handleRef) {

                THandle entityHandle = handleRef.get<THandle>();

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


            [[nodiscard]] size_t activeCount() const noexcept {
                return activeEntities_.size();
            }

            [[nodiscard]] size_t inactiveCount() const noexcept {
                return inactiveEntities_.size();
            }


            [[nodiscard]] HandleSpanRef  inactiveEntities() const noexcept override{
                return HandleSpanRef{inactiveEntities_};
            };


            [[nodiscard]] HandleSpanRef activeEntities() const noexcept override {
                return HandleSpanRef{activeEntities_};
            };
        };


        std::unique_ptr<Concept> model_;

        HandleTypeId handleTypeId_;

        template<typename THandle>
        void assertTypeId() {
            assert(handleTypeId_ == HandleTypeId::template id<THandle>() && "HandleTypeId not maintained by this pool.");
        }

    public:

        EntityPool(const EntityPool&) = delete;
        EntityPool& operator=(const EntityPool&) = delete;
        EntityPool(EntityPool&&) noexcept = default;
        EntityPool& operator=(EntityPool&&) noexcept = default;

        template<typename THandle>
        EntityPool() : model_(std::make_unique<Model<THandle>>()),
        handleTypeId_(HandleTypeId::template id<THandle>()) {}

        template<typename THandle>
        [[nodiscard]] std::span<const THandle> activeEntities() {
            assertTypeId<THandle>();
            return model_->activeEntities().get<THandle>();
        };

        template<typename THandle>
        [[nodiscard]] std::span<const THandle> inactiveEntities() {
            assertTypeId<THandle>();
            return model_->inactiveEntities().get<THandle>();
        };

    };


}
