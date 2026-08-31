/**
 * @file EntityPool.ixx
 * @brief Object pool for efficient Entity management and recycling.
 */
module;

#include <memory>
#include <vector>
#include <cassert>
#include <limits>
#include <span>
#include <optional>

export module helios.engine.runtime.pooling.EntityPool;

import helios.core.common.types;
import helios.ecs;
import helios.engine.runtime.pooling.types;
import helios.core.log;


#define HELIOS_LOG_SCOPE "helios::engine::runtime::pooling::EntityPool"
export namespace helios::engine::runtime::pooling {




    class EntityPool {

        using HandleTypeId = ecs::common::types::HandleTypeId;
        using EntityHandleValue = ecs::common::types::EntityHandleValue;
        using HandleSpanRef = ecs::common::types::HandleSpanRef;
        using EntityRef = ecs::EntityRef;
        using ConstEntityRef = ecs::ConstEntityRef;
        using EntityManagerRef = ecs::EntityManagerRef;
        using EntityPoolTypeId = types::EntityPoolTypeId;
        using TypeId_t = core::common::types::TypeId_t;

        class Concept {

        public:

            virtual ~Concept() = default;

            [[nodiscard]] virtual EntityRef prefabEditor() noexcept = 0;
            [[nodiscard]] virtual ConstEntityRef prefab() const noexcept = 0;
            virtual bool setPoolSize(size_t poolSize) = 0;
            virtual bool prewarm(EntityManagerRef entityManagerRef) = 0;
            [[nodiscard]] virtual size_t size() const noexcept = 0;
            [[nodiscard]] virtual HandleSpanRef acquire(std::size_t amount) = 0;
            [[nodiscard]] virtual bool isLocked() const noexcept = 0;
            [[nodiscard]] virtual bool lock() noexcept = 0;
            virtual bool release(EntityHandleValue handleValue) = 0;
            virtual bool releaseAndRemove(EntityHandleValue handleValue) = 0;
            [[nodiscard]] virtual size_t activeCount() const noexcept = 0;
            [[nodiscard]] virtual size_t inactiveCount() const noexcept = 0;
            [[nodiscard]] virtual HandleSpanRef inactiveEntities() const noexcept = 0;
            [[nodiscard]] virtual HandleSpanRef activeEntities() const noexcept  = 0;

        };


        template<typename THandle>
        class Model final : public Concept {

            static inline auto& logger_ = helios::core::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);


            using HandleType = THandle;

            ecs::entity::EntityManager<THandle> entityManager_;

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


            HandleSpanRef acquireImpl(std::size_t amount) {

                if (!locked_) {
                    logger_.error("Pool must be locked before acquiring objects");
                    assert(false && "Pool must be locked before acquiring objects");
                    return HandleSpanRef::makeEmpty<THandle>();
                }

                if (amount == 0 || inactiveEntities_.empty()) {
                    return HandleSpanRef::makeEmpty<THandle>();
                }

                const std::size_t start = activeEntities_.size();
                const std::size_t count = std::min(inactiveEntities_.size(), amount);

                for (std::size_t i = 0; i < count; i++) {
                    THandle entityHandle = inactiveEntities_.back();
                    inactiveEntities_.pop_back();

                    auto idx = entityHandle.entityId() - delta_;

                    if (activeIndex_.size() <= idx) {
                        activeIndex_.resize(idx + 1, helios::ecs::common::types::EntityTombstone);
                        versionIndex_.resize(idx + 1, helios::ecs::common::types::EntityTombstone);
                    }

                    activeIndex_[idx] = activeEntities_.size();
                    versionIndex_[idx] = entityHandle.versionId();

                    activeEntities_.push_back(entityHandle);

                    amount--;
                }
                return HandleSpanRef{
                    std::span<const THandle>{
                        activeEntities_.begin() + start, activeEntities_.end()
                    }
                };
            }

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

        public:

            explicit Model(std::optional<std::size_t> poolSize = std::nullopt) {
                if (poolSize.has_value()) {
                    setPoolSize(poolSize.value());
                }
            }

            [[nodiscard]] EntityRef prefabEditor() noexcept {
                if (!prefabHandle_.isValid()) {
                    prefabHandle_ = entityManager_.create();
                }

                return EntityRef{prefabHandle_, entityManager_};
            }

            [[nodiscard]] ConstEntityRef prefab() const noexcept {
                return ConstEntityRef{prefabHandle_, entityManager_};
            }

            bool setPoolSize(const std::size_t poolSize) override {

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

            bool prewarm(EntityManagerRef entityManagerRef) override {

                auto& targetEntityManager = entityManagerRef.get<THandle>();

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
                        ecs::entity::Entity<ecs::entity::EntityManager<THandle>> targetEntity{targetHandle, &targetEntityManager};
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

            [[nodiscard]] size_t size() const noexcept override {
                return poolSize_;
            }



            [[nodiscard]] HandleSpanRef acquire(const std::size_t amount) override {
                return acquireImpl(amount);
            }


            [[nodiscard]] bool isLocked() const noexcept override {
                return locked_;
            }


            [[nodiscard]] bool lock() noexcept override {

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
                } catch (const std::exception&) {
                    logger_.error("Could not lock pool, resiszing failed.");
                    assert(false && "Resizing failed.");
                    return false;
                }

                locked_ = true;

                return true;
            }





            bool release(const EntityHandleValue handleValue) override {

                THandle entityHandle = handleValue.get<THandle>();


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

            bool releaseAndRemove(const EntityHandleValue handleValue) override {

                THandle entityHandle = handleValue.get<THandle>();

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


            [[nodiscard]] size_t activeCount() const noexcept override {
                return activeEntities_.size();
            }

            [[nodiscard]] size_t inactiveCount() const noexcept override {
                return inactiveEntities_.size();
            }


            [[nodiscard]] HandleSpanRef  inactiveEntities() const noexcept override{
                return HandleSpanRef{inactiveEntities_};
            };


            [[nodiscard]] HandleSpanRef activeEntities() const noexcept override {
                return HandleSpanRef{activeEntities_};
            };
        };


        template<typename THandle>
        void assertTypeId() const {
            assert(handleTypeId_ == HandleTypeId::id<THandle>() && "HandleTypeId not maintained by this pool.");
        }

        template<typename THandle>
        EntityPool(std::in_place_type_t<THandle>, std::optional<std::size_t> poolSize)
        : model_(std::make_unique<Model<THandle>>(poolSize)),
        handleTypeId_(HandleTypeId::id<THandle>()) {}

        std::unique_ptr<Concept> model_;


        HandleTypeId handleTypeId_;

    public:

        EntityPool(const EntityPool&) = delete;
        EntityPool& operator=(const EntityPool&) = delete;
        EntityPool(EntityPool&&) noexcept = default;
        EntityPool& operator=(EntityPool&&) noexcept = default;

        template<typename THandle>
        static constexpr EntityPool make(std::optional<std::size_t> poolSize = std::nullopt) {
            return EntityPool{std::in_place_type<THandle>, poolSize};
        }



        template<typename THandle>
        [[nodiscard]] ecs::entity::Entity<ecs::entity::EntityManager<THandle>> prefabEditor() noexcept {
            assertTypeId<THandle>();
            return model_->prefabEditor().get<THandle>();
        }

        template<typename THandle>
        [[nodiscard]] ecs::entity::Entity<const ecs::entity::EntityManager<THandle>> prefab() const noexcept {
            assertTypeId<THandle>();
            return model_->prefab().get<THandle>();
        }

        bool setPoolSize(const std::size_t poolSize) {
            return model_->setPoolSize(poolSize);
        }

        bool prewarm(const EntityManagerRef entityManagerRef) {
            return model_->prewarm(entityManagerRef);
        }

        [[nodiscard]] size_t size() const noexcept {
            return model_->size();
        }

        template<typename THandle>
        [[nodiscard]] std::span<const THandle> acquire(const std::size_t amount) {
            assertTypeId<THandle>();
            return model_->acquire(amount).get<THandle>();
        }

        [[nodiscard]] bool isLocked() const noexcept {
            return model_->isLocked();
        }

        [[nodiscard]] bool lock() noexcept {
            return model_->lock();
        }


        template<typename THandle>
        bool release(const THandle entityHandle) {
            assertTypeId<THandle>();
            return model_->release(EntityHandleValue{entityHandle});
        }

        template<typename THandle>
        bool releaseAndRemove(const THandle entityHandle) {
            assertTypeId<THandle>();
            return model_->releaseAndRemove(EntityHandleValue{entityHandle});
        }

        [[nodiscard]] size_t activeCount() const noexcept {
            return model_->activeCount();
        }

        [[nodiscard]] size_t inactiveCount() const noexcept {
            return model_->inactiveCount();
        }

        template<typename THandle>
        [[nodiscard]] std::span<const THandle> activeEntities() const {
            assertTypeId<THandle>();
            return model_->activeEntities().get<THandle>();
        };

        template<typename THandle>
        [[nodiscard]] std::span<const THandle> inactiveEntities() const {
            assertTypeId<THandle>();
            return model_->inactiveEntities().get<THandle>();
        };

        [[nodiscard]] EntityPoolTypeId typeId() const noexcept {
            return EntityPoolTypeId{handleTypeId_.value()};
        }


    };


}
