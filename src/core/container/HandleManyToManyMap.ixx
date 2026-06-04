/**
 * @file HandleManyToManyMap.ixx
 * @brief Dense association map from one handle to multiple handles.
 */
module;

#include <vector>
#include <span>
#include <cassert>
#include <concepts>
#include <iterator>
#include <cstddef>

export module helios.engine.core.container.HandleManyToManyMap;

import helios.ecs.types.EntityHandle;

using namespace helios::ecs;
export namespace helios::engine::core::container {

    /**
     * @brief Stores associations from one handle domain to many handles.
     * @tparam TOneHandle Handle type used as association key.
     * @tparam TManyHandle Handle type stored as associated value.
     *
     * @details Uses `entityId` as direct index into dense storage.
     */
    template <typename TOneHandle, typename TManyHandle>
    class HandleManyToManyMap {

        /**
         * @brief Dense storage: key `entityId` -> associated values.
         */
        std::vector<std::vector<TManyHandle>> values_;

    public:


        /**
         * @brief Binds two entities by their handle types.
         * @tparam TOneEntity Entity type exposing `TOneHandle`.
         * @tparam TManyEntity Entity type exposing `TManyHandle`.
         * @param oneEntity Entity providing the key handle.
         * @param manyEntity Entity providing the associated value handle.
         * @return `true` after the association was appended.
         */
        template<typename TOneEntity, typename TManyEntity>
        requires std::same_as<TOneHandle, typename TOneEntity::Handle_type> && std::same_as<TManyHandle, typename TManyEntity::Handle_type>
        bool bind(TOneEntity oneEntity, TManyEntity manyEntity) {
            return bind(oneEntity.handle(), manyEntity.handle());
        }


        /**
         * @brief Appends an association from key handle to value handle.
         * @param key Key handle on the left side.
         * @param value Value handle on the right side.
         * @return `true` after the association was appended.
         */
        bool bind(const TOneHandle key, const TManyHandle value) {

            const auto keyIdx = key.entityId;
            const auto valueIdx = value.entityId;

            // one idx to many handles
            if (values_.size() <= keyIdx) {
                values_.resize(keyIdx + 1);
            }
            values_[keyIdx].push_back(value);


            return true;
        }


        /**
         * @brief Returns all values associated with a key handle.
         * @param key Key handle to query.
         * @return Read-only span of associated handles, or empty span if none exist.
         */
        [[nodiscard]] std::span<const TManyHandle> values(const TOneHandle key) const noexcept {

            const auto keyIdx = key.entityId;

            if (values_.size() <= keyIdx) {
                return {};
            }

            return values_[keyIdx];
        }

        /**
         * @brief Removes all stored associations.
         */
        void clear() {
            values_.clear();
        }


        /**
         * @brief Reserves capacity for key-indexed storage.
         * @param keyCapacity Expected number of key slots.
         */
        void reserve(std::size_t keyCapacity) {
            values_.reserve(keyCapacity);
        }

    };

}