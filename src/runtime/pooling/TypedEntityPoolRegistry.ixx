/**
 * @file TypedEntityPoolRegistry.ixx
 * @brief Central registry for managing multiple EntityPool instances.
 */
module;

#include <memory>
#include <unordered_map>
#include <cassert>

export module helios.engine.runtime.pooling.TypedEntityPoolRegistry;

import helios.engine.runtime.pooling.EntityPool;
import helios.engine.runtime.pooling.types;
import helios.engine.util.log;

using namespace helios::engine::runtime::pooling::types;
#define HELIOS_LOG_SCOPE "helios::engine::runtime::pooling::TypedEntityPoolRegistry"
export namespace helios::engine::runtime::pooling {


    /**
     * @brief Typed registry that manages multiple `EntityPool` instances keyed by `EntityPoolId`.
     *
     * Pools are stored in a `std::tuple` of `std::unordered_map`s, one map per managed handle type.
     * All pool operations are dispatched at compile time via the `THandle` template argument.
     *
     * @tparam TManagedHandles  Pack of handle types whose pools are managed by this registry.
     */
    template<typename ...TManagedHandles>
    class TypedEntityPoolRegistry {

    private:

        /**
         * @brief Maps pool IDs to their corresponding EntityPool instances.
         */
        std::tuple<std::unordered_map<EntityPoolId<TManagedHandles>, EntityPool<TManagedHandles>>...> pools_;

        template<typename THandle>
        [[nodiscard]] std::unordered_map<EntityPoolId<THandle>, EntityPool<THandle>>& managedPoolSet() {
            return std::get<std::unordered_map<EntityPoolId<THandle>, EntityPool<THandle>>>(pools_);
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
         * @details The pool is move-constructed into the internal map.
         * If a pool with the given ID already exists, an assertion is triggered and a warning is logged.
         *
         * @tparam THandle  Handle type of the pool.
         * @param id        Unique identifier for the pool.
         * @param entityPool Pool to add; its contents are moved into the registry.
         *
         * @return Reference to the stored pool.
         */
        template<typename THandle>
        EntityPool<THandle>& addPool(const EntityPoolId<THandle> id, EntityPool<THandle>& entityPool) noexcept {

            auto [it, inserted] = pools_.try_emplace(id, std::move(entityPool));

            if (!inserted) {
                logger_.warn("EntityPoolId {0} already registered with this registry", id.value());
                assert(false && "EntityPoolId already registered with this registry");
            }

            return it->second;
        }


        /**
         * @brief Retrieves a pool by its ID (const).
         *
         * @tparam THandle  Handle type of the pool.
         * @param id        Identifier of the pool to retrieve.
         *
         * @return Pointer to the pool, which may be nullptr.
         *
         * @note Triggers an assertion if the pool is not registered.
         */
        template<typename THandle>
        [[nodiscard]] const EntityPool<THandle>* pool(const EntityPoolId<THandle> id) const {

            const auto it = managedPoolSet<THandle>().find(id);

            if (it == managedPoolSet<THandle>().end()) {
                assert(false && "EntityPoolId not registered with this registry");
                logger_.error("EntityPoolId not registered with this registry");
                return nullptr;
            }

            return *it->second;
        }

        /**
         * @brief Retrieves a pool by its ID.
         *
         * @tparam THandle  Handle type of the pool.
         * @param id        Identifier of the pool to retrieve.
         *
         * @return Pointer to the pool, which may be nullptr.
         *
         * @note Triggers an assertion if the pool is not registered.
         */
        template<typename THandle>
        [[nodiscard]] EntityPool<THandle>* pool(const helios::engine::runtime::pooling::types::EntityPoolId<THandle> id) {

            const auto it = managedPoolSet<THandle>().find(id);

            if (it == managedPoolSet<THandle>().end()) {
                assert(false && "EntityPoolId not registered with this registry");
                logger_.error("EntityPoolId not registered with this registry");
                return nullptr;
            }

            return &it->second;
        }

        /**
         * @brief Returns the full pool map for a given handle type.
         *
         * @tparam THandle  Handle type whose pool map to retrieve.
         *
         * @return Reference to the `unordered_map` mapping `EntityPoolId<THandle>` to `EntityPool<THandle>`.
         */
        template<typename THandle>
        [[nodiscard]] std::unordered_map<EntityPoolId<THandle>, EntityPool<THandle>>& pools() {
            return managedPoolSet<THandle>();
        }


        /**
         * @brief Checks whether a pool with the given ID is registered.
         *
         * @tparam THandle  Handle type of the pool.
         * @param id        Identifier of the pool to check.
         *
         * @return `true` if the pool exists, `false` otherwise.
         */
        template<typename THandle>
        [[nodiscard]] bool has(const helios::engine::runtime::pooling::types::EntityPoolId<THandle> id) const noexcept {
            return managedPoolSet<THandle>().contains(id);
        }


    };

}
