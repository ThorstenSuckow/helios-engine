/** @file EntityPoolKey.ixx
 * @brief Key containing numeric index and strongly-typed identifier for EntityPool instances.
 */
module;

#include <string>
#include <cstddef>

export module helios.engine.runtime.pooling.types:EntityPoolKey;

import :EntityPoolId;
export namespace helios::engine::runtime::pooling::types {

    /**
     * @brief Composite key identifying an `EntityPool` slot by numeric index and strongly-typed `EntityPoolId`.
     *
     * Constructed via `EntityPoolKey(idx, id)`; default-constructed keys are invalid (`isValid() == false`).
     *
     * @tparam THandle  Handle type of the associated pool.
     */
    template<typename THandle>
    class EntityPoolKey {

        /**
         * @brief Slot index used for O(1) access in the pool vector.
         */
        std::size_t idx_;

        /**
         * @brief Strongly-typed string identifier for the pool.
         */
        EntityPoolId<THandle> entityPoolId_;

        /**
         * @brief `false` for default-constructed keys, `true` after explicit construction.
         */
        bool isValid_ = false;

    public:

        /**
         * @brief Constructs an invalid key.
         */
        EntityPoolKey() = default;

        /**
         * @brief Constructs a valid key.
         *
         * @param idx  Slot index in the owning registry's pool vector.
         * @param entityPoolId Identifier used for the associated pool.
         */
        explicit EntityPoolKey(const std::size_t idx, EntityPoolId<THandle> entityPoolId) :
                idx_(idx), entityPoolId_(entityPoolId), isValid_(true) {}


        /**
         * @return `true` if this key was constructed with an explicit index and ID.
         */
        [[nodiscard]] bool isValid() const noexcept {
            return isValid_;
        }

        /**
         * @return Slot index in the owning registry's pool vector.
         */
        [[nodiscard]] std::size_t index() const noexcept {
            return idx_;
        }

        /**
         * @return Const reference to the strongly-typed pool identifier.
         */
        [[nodiscard]] const EntityPoolId<THandle>& entityPoolId() const noexcept {
            return entityPoolId_;
        }


        /**
         * @brief Equality is based on all members (`idx_`, `entityPoolId_`, `isValid_`).
         */
        bool operator==(const EntityPoolKey& other) const noexcept = default;
    };


};
