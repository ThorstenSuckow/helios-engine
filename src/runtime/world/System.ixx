/**
 * @file System.ixx
 * @brief Type-erased system wrapper using the Concept/Model pattern.
 */
module;

#include <cassert>
#include <memory>
#include <optional>

export module helios.engine.runtime.world.System;


import helios.engine.runtime.messaging.command.concepts.IsCommandBufferLike;
import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.timing.TimerManager;

import helios.engine.runtime.world.concepts;
import helios.engine.runtime.messaging.command.types.CommandBufferTypeId;

using namespace helios::engine::runtime::timing;
using namespace helios::engine::runtime::messaging::command::types;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::messaging::command::concepts;
using namespace helios::engine::runtime::world::concepts;
export namespace helios::engine::runtime::world {


    /**
     * @brief Type-erased wrapper for game logic processors.
     *
     * @details System uses the Concept/Model pattern to erase the concrete
     * system type. Concrete systems are plain classes that satisfy
     * `IsSystemLike<T>` and do not inherit from System.
     *
     * The internal `Concept` base defines the virtual update interface, and
     * `Model<T>` adapts the concrete type T, owning it by value.
     *
     * If a system exposes `CommandBuffer_type`, `System` calls
     * `update(UpdateContext&, CommandBuffer_type&)` and uses the injected
     * buffer pointer supplied by the creator (typically via Pass registration).
     *
     * System is move-only (non-copyable).
     *
     * @see IsSystemLike
     * @see SystemRegistry
     *
     * @see https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Type_Erasure
     */
    class System {

    private:
        /**
         * @brief Internal virtual interface for type erasure.
         */
        class Concept {
        public:

            virtual ~Concept() = default;
            virtual void update(UpdateContext& updateContext) noexcept = 0;
            virtual void flush(UpdateContext& updateContext) noexcept = 0;
            virtual void init(GameWorld& gameWorld) noexcept = 0;
            [[nodiscard]] virtual void* underlying() noexcept = 0;
            [[nodiscard]] virtual const void* underlying() const noexcept = 0;
        };

        /**
         * @brief Typed wrapper that adapts a concrete system to the Concept interface.
         *
         * @tparam T The concrete system type, must satisfy `IsSystemLike<T>`.
         */
        template<typename T>
        class Model final : public Concept {
            T system_;
            void* injectedBuffer_{nullptr};
        public:

            explicit Model(T sys, void* buffer = nullptr)
                    : system_(std::move(sys)), injectedBuffer_(buffer) {}

            void update(UpdateContext& ctx) noexcept override {
                if constexpr (requires { typename T::CommandBuffer_type; }) {
                    using BufT = typename T::CommandBuffer_type;
                    system_.update(ctx, *static_cast<BufT*>(injectedBuffer_));
                } else {
                    system_.update(ctx);
                }
            }

            void init(GameWorld& gameWorld) noexcept override {
                // noop
            }

            void flush(UpdateContext& ctx) noexcept override {
                if constexpr (requires { typename T::CommandBuffer_type; }) {
                    static_cast<typename T::CommandBuffer_type*>(injectedBuffer_)->flush(ctx);
                }
            }

            void* underlying() noexcept override {
                return &system_;
            }

            const void* underlying() const noexcept override {
                return &system_;
            }
        };

        template<typename T, typename TCommandBuffer>
        class ModelWithOwnedCommandBuffer final : public Concept {
            T system_;
            TCommandBuffer commandBuffer_;
        public:

            explicit ModelWithOwnedCommandBuffer(T sys, TCommandBuffer cb)
                    : system_(std::move(sys)), commandBuffer_(std::move(cb)) {}

            void update(UpdateContext& ctx) noexcept override {
                system_.update(ctx, commandBuffer_);
            }

            void init (GameWorld& gameWorld) noexcept override {
                commandBuffer_.init(
                    gameWorld.commandHandlerRegistry(),
                    gameWorld.managerRegistry()
                );
            }

            void flush(UpdateContext& updateContext) noexcept override {
                commandBuffer_.flush(updateContext);
            }

            void* underlying() noexcept override {
                return &system_;
            }

            const void* underlying() const noexcept override {
                return &system_;
            }
        };

        std::unique_ptr<Concept> pimpl_;

    public:

        /**
         * @brief Default constructor creating an empty System.
         */
        System() = default;

        /**
         * @brief Wraps a concrete system in a type-erased System.
         *
         * @tparam T The concrete system type, must satisfy `IsRuntimeSystemLike<T>`.
         *
         * @param system The concrete system instance to wrap (moved into internal storage).
         * @param buffer Optional pointer to the concrete `T::CommandBuffer_type`
         *        instance used for systems with two-parameter update signatures.
         */
        template<typename T>
        requires IsRuntimeSystemLike<std::remove_cvref_t<T>>
        explicit System(T&& system, void* buffer = nullptr)
             : pimpl_(std::make_unique<Model<std::remove_cvref_t<T>>>(std::forward<T>(system), buffer)){}

        /**
         * @brief Wraps a concrete system in a type-erased System that also owns a CommandBuffer.
         *
         * @tparam T The concrete system type, must satisfy `IsRuntimeSystemLike<T>`.
         * @tparam TCommandBuffer The concrete CommandBuffer-type, must satisfy `IsCommandBufferLike<TCommandBuffer>`.
         *
         * @param system The concrete system instance to wrap (moved into internal storage).
         * @param commandBuffer The concrete command buffer instance to wrap (moved into internal storage).
         */
        template<typename T, typename TCommandBuffer>
        requires IsRuntimeSystemLike<std::remove_cvref_t<T>>
            && IsCommandBufferLike<std::remove_cvref_t<TCommandBuffer>>
            && (!std::is_lvalue_reference_v<TCommandBuffer>)
        explicit System(T&& system, TCommandBuffer&& commandBuffer)
            : pimpl_(
                std::make_unique<
                    ModelWithOwnedCommandBuffer<
                        std::remove_cvref_t<T>,
                        std::remove_cvref_t<TCommandBuffer>
                    >
                >(
                    std::forward<T>(system),
                    std::forward<TCommandBuffer>(commandBuffer)
                ))
        {}

        System(const System&) = delete;
        System& operator=(const System&) = delete;

        System& operator=(System&&) = default;
        System(System&&) noexcept = default;

        /**
         * @brief Delegates to the wrapped system's `update()` method.
         *
         * @param updateContext The current frame's update context.
         *
         * @pre System must be initialized (pimpl_ != nullptr).
         */
        void update(UpdateContext& updateContext) noexcept {
            assert(pimpl_ && "System not initialized");
            pimpl_->update(updateContext);
        }

        /**
         * @brief Flushes any outstanding changes, e.g. commands of the associated buffer.
         *
         * @param updateContext The current frame's update context.
         */
        void flush(UpdateContext& updateContext) noexcept {
            assert(pimpl_ && "System not initialized");
            pimpl_->flush(updateContext);
        }

        /**
         * @brief Inits this system with the owning GameWorld.
         *
         * @param gameWorld The owning GameWorld.
         */
        void init(GameWorld& gameWorld) noexcept {
            assert(pimpl_ && "System not initialized");
            pimpl_->init(gameWorld);
        }

        /**
         * @brief Returns a type-erased pointer to the wrapped system instance.
         *
         * @return Pointer to the underlying concrete system.
         *
         * @pre System must be initialized (pimpl_ != nullptr).
         */
        [[nodiscard]] void* underlying() noexcept {
            assert(pimpl_ && "System not initialized");
            return pimpl_->underlying();
        }

        /**
         * @copydoc underlying()
         */
        [[nodiscard]] const void* underlying() const noexcept {
            assert(pimpl_ && "System not initialized");
            return pimpl_->underlying();
        }

    };


}