/**
 * @file TypedEntityPoolRegistry.ixx
 * @brief Central registry for managing multiple EntityPool instances.
 */
module;

#include <cassert>
#include <functional>
#include <optional>
#include <utility>
#include <vector>
#include <tuple>
#include <type_traits>

export module helios.engine.runtime.pooling.TypedEntityPoolRegistry;

import helios.engine.runtime.pooling.EntityPool;
import helios.engine.runtime.pooling.types;
import helios.engine.util.log;

using namespace helios::engine::runtime::pooling::types;
#define HELIOS_LOG_SCOPE "helios::engine::runtime::pooling::TypedEntityPoolRegistry"
export namespace helios::engine::runtime::pooling {


    /**
     * @brief Typed registry that manages multiple `EntityPool` instances keyed by `EntityPoolKey`.
     *
     * Pools are stored in index-based `vector` slots (one `vector` per managed handle type, held in a `std::tuple`).
     * All pool operations are dispatched at compile time via the `THandle` template argument.
     *
     * @tparam TManagedHandles  Pack of handle types whose pools are managed by this registry.
     */
    template<typename ...TManagedHandles>
    class TypedEntityPoolRegistry {

    private:

        /**
         * @brief Aggregates a pool together with its registration key in a single slot.
         *
         * @tparam THandle  Handle type of the managed pool.
         */
        template<typename THandle>
        struct EntityPoolSlot {
            EntityPoolKey<THandle> key;
            EntityPool<THandle> pool;
        };

        /**
         * @brief Index-addressable slot storage; each slot holds an `EntityPoolSlot` or `std::nullopt`.
         * The slot index corresponds to `EntityPoolKey::idx()`.
         */
        std::tuple<std::vector<std::optional<EntityPoolSlot<TManagedHandles>>>...> pools_;

        /**
         * @brief Ordered list of registered keys; used for iteration in `forEach`.
         */
        std::tuple<std::vector<EntityPoolKey<TManagedHandles>>...> availablePoolKeys_;

        /** @brief Returns the mutable slot vector for `THandle`. */
        template<typename THandle>
        [[nodiscard]] std::vector<std::optional<EntityPoolSlot<THandle>>>& managedPoolVector() noexcept {
            return std::get<std::vector<std::optional<EntityPoolSlot<THandle>>>>(pools_);
        }

        /** @brief Returns the const slot vector for `THandle`. */
        template<typename THandle>
        [[nodiscard]] const std::vector<std::optional<EntityPoolSlot<THandle>>>& managedPoolVector() const noexcept {
            return std::get<std::vector<std::optional<EntityPoolSlot<THandle>>>>(pools_);
        }

        static inline auto& logger_ = helios::engine::util::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);

    public:

        /**
         * @brief Default constructor.
         */
        TypedEntityPoolRegistry() = default;


        /**
         * @brief Adds a new pool to the registry.
         *
         * @details The pool is move-constructed into the slot at `key.idx()`.
         * Returns `nullptr` and triggers an assertion if the key is invalid or the slot is already occupied.
         *
         * @tparam THandle   Handle type of the pool.
         * @param key        Key containing the slot index and strongly-typed identifier for the pool.
         * @param entityPool Pool to add (moved into the registry).
         *
         * @return Pointer to the stored pool, or `nullptr` on failure.
         */
        template<typename THandle>
        EntityPool<THandle>* addPool(const EntityPoolKey<THandle> key, EntityPool<THandle>&& entityPool) {

            if (!key.isValid()) {
                logger_.error("Invalid key passed.");
                assert(false && "Invalid key passed.");
                return nullptr;
            }

            auto& poolSlots = managedPoolVector<THandle>();
            auto& availablePoolKeys = std::get<std::vector<EntityPoolKey<THandle>>>(availablePoolKeys_);

            const auto idx = key.idx();

            if (poolSlots.size() <= idx) {
                poolSlots.resize(idx + 1);
            }
            
            if (poolSlots[idx]) {
                logger_.error("EntityPoolKey with this index already registered.");
                assert(false && "EntityPoolKey with this index already registered.");
                return nullptr;
            }

            poolSlots[idx].emplace(EntityPoolSlot<THandle>{key, std::move(entityPool)});
            availablePoolKeys.push_back(key);

            return &(poolSlots[idx]->pool);
        }

        /**
         * @brief Invokes `func` for every registered pool of the given handle type.
         *
         * @tparam THandle  Handle type of the pools to iterate.
         * @tparam TFunc    Callable with signature `void(EntityPool<THandle>&)`.
         *
         * @param func Function to invoke for each pool.
         */
        template<typename THandle, typename TFunc>
        requires std::invocable<TFunc&, EntityPool<THandle>&>
        void forEach(TFunc&& func) noexcept(std::is_nothrow_invocable_v<TFunc&, EntityPool<THandle>&>) {

            auto& poolSlots = managedPoolVector<THandle>();
            const auto& availablePoolKeys = std::get<std::vector<EntityPoolKey<THandle>>>(availablePoolKeys_);

            for (const auto& key : availablePoolKeys) {
                auto& poolSlot = poolSlots[key.idx()];
                if (!poolSlot) {
                    logger_.error("pool slot was not valid.");
                    assert(false && "pool slot was not valid");
                    continue;
                }
                std::invoke(func, poolSlot->pool);
            }
        }

        /**
         * @brief Retrieves a pool by key (const).
         *
         * @tparam THandle  Handle type of the pool.
         * @param key       Key identifying the pool slot.
         *
         * @return Const pointer to the pool, or `nullptr` if the key is invalid or not registered.
         */
        template<typename THandle>
        [[nodiscard]] const EntityPool<THandle>* pool(const EntityPoolKey<THandle> key) const {
            if (!key.isValid()) {
                logger_.error("Invalid key passed.");
                assert(false && "Invalid key passed.");
                return nullptr;
            }

            auto& poolSlots = managedPoolVector<THandle>();

            const auto idx = key.idx();

            if (poolSlots.size() <= idx ||
                !poolSlots[idx] ||
                poolSlots[idx]->key != key) {
                logger_.error("EntityPoolKey not registered with this registry");
                assert(false && "EntityPoolKey not registered with this registry");
                return nullptr;
            }

            return &poolSlots[idx]->pool;
        }

        /**
         * @brief Retrieves a pool by key.
         *
         * @tparam THandle  Handle type of the pool.
         * @param key       Key identifying the pool slot.
         *
         * @return Pointer to the pool, or `nullptr` if the key is invalid or not registered.
         */
        template<typename THandle>
        [[nodiscard]] EntityPool<THandle>* pool(const EntityPoolKey<THandle> key) {
            return const_cast<EntityPool<THandle>*>(std::as_const(*this).template pool<THandle>(key));
        }


        /**
         * @brief Checks if the pool exists for the specified key.
         *
        * @tparam THandle  Handle type of the pool.
         * @param key       Key identifying the pool slot.
         *
         * @return True if a pool was registered with this key, false otherwise.
         */
        template<typename THandle>
        [[nodiscard]] bool has(const EntityPoolKey<THandle> key) const {
            if (!key.isValid()) {
                return false;
            }

            auto& poolSlots = managedPoolVector<THandle>();

            const auto idx = key.idx();

            if (poolSlots.size() <= idx || !poolSlots[idx] || poolSlots[idx]->key != key) {
                return false;
            }

            return true;
        }
    };

}
