/**
 * @file ContextProvider.ixx
 * @brief Provides access to type-erased access to runtime context objects.
 */
module;

#include <memory>
#include <cassert>
#include <exception>

export module helios.engine.runtime.world.ContextProvider;

import helios.ecs.common;

import helios.engine.runtime.world.UpdateContext;

export namespace helios::engine::runtime::world {


    /**
     * @todo ontexts should be frame-local and, where possible, materialized before parallel
     * system execution. This keeps context lookup read-only during parallel execution
     * and avoids concurrent mutation of the TypeMap.
     *
     * Shared mutable context state must still be synchronized or avoided.
     *
     * @param typeId
     * @param updateContext
     * @return
     */
    class ContextProvider {
        
        using ContextRef = ecs::common::types::ContextRef;
        using ContextTypeId = ecs::common::types::ContextTypeId;

        class Concept {
        public:
            virtual ~Concept() = default;

            virtual ContextRef get(ContextTypeId typeId, UpdateContext& updateContext) = 0;

            virtual ContextRef get(ContextTypeId typeId) = 0;

            /**
             * @brief Requests the concrete provider to clear its context storage, e.g. when the frame was discarded.
             * @return
             */
            virtual bool clear() = 0;

        };

        template<typename TConcreteProvider>
        class Model final : public Concept {

            TConcreteProvider provider_;

        public:

            explicit Model(TConcreteProvider&& provider)
                : provider_(std::move(provider)) {}

            ContextRef get(ContextTypeId typeId, UpdateContext& updateContext) override {;
                return provider_.get(typeId, updateContext);
            }

            ContextRef get(ContextTypeId typeId) override {;
                return provider_.get(typeId);
            }

            bool clear() override {
                return provider_.clear();
            }

            TConcreteProvider& provider() { return provider_; }
        };

        std::unique_ptr<Concept> pimpl_;

    public:

        ContextProvider() = delete;
        ContextProvider(const ContextProvider&) = delete;
        ContextProvider& operator=(const ContextProvider&) = delete;
        ContextProvider(ContextProvider&&) noexcept = default;
        ContextProvider& operator=(ContextProvider&&) noexcept = default;

        template<typename TConcreteProvider>
        requires (!std::is_lvalue_reference_v<TConcreteProvider>)
        explicit ContextProvider(TConcreteProvider&& concreteProvider) :
        pimpl_(std::make_unique<Model<std::remove_cvref_t<TConcreteProvider>>>(std::forward<TConcreteProvider>(concreteProvider)))
        {}


        [[nodiscard]] ContextRef get(const ContextTypeId typeId, UpdateContext& updateContext) {
            return pimpl_->get(typeId, updateContext);
        }

        [[nodiscard]] ContextRef get(const ContextTypeId typeId) {
            return pimpl_->get(typeId);
        }

        template<typename TConcreteContext>
        [[nodiscard]] TConcreteContext& get() {

            auto typeId = ContextTypeId::template id<TConcreteContext>();
            auto& ref = get(typeId);

            auto* ctx = ref.template tryGet<TConcreteContext>();

            if (!ctx) [[unlikely]] {
                assert(false && "could not get context");
                std::terminate();
            }
            return *ctx;
        }


        bool clear() {
            return pimpl_->clear();
        }

    };

}