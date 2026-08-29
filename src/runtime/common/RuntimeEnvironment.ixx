/**
 * @file RuntimeEnvironment.ixx
 * @brief Runtime environment facade for platform readiness state.
 */
module;

#include <optional>
#include <vector>
#include <cassert>

export module helios.engine.runtime.common.RuntimeEnvironment;

import helios.ecs;

import helios.engine.platform.environment.components;


using namespace helios::ecs;
using namespace helios::engine::platform::environment;
using namespace helios::engine::platform::environment::components;

export namespace helios::engine::runtime::common {


    /**
     * @brief Facade around the platform-environment entity used by runtime systems.
     */
    class RuntimeEnvironment {

    private:


        struct RuntimeDomainTag{};
        using RuntimeHandle = ecs::common::types::EntityHandle<RuntimeDomainTag>;
        using EntityManager = ecs::EntityManager<RuntimeHandle>;
        using RuntimeObject = ecs::Entity<ecs::EntityManager<RuntimeHandle>>;
        using ConstRuntimeObject = ecs::Entity<const ecs::EntityManager<RuntimeHandle>>;

        EntityManager entityManager_;

        RuntimeHandle runtimeHandle_;
        
        [[nodiscard]] RuntimeObject runtimeObject() noexcept {
            return RuntimeObject{runtimeHandle_, &entityManager_};
        };

        [[nodiscard]] ConstRuntimeObject runtimeObject() const noexcept {
            return ConstRuntimeObject{runtimeHandle_, &entityManager_};
        }

    
    public:


        RuntimeEnvironment() : runtimeHandle_(entityManager_.create()) {}

        /**
         * @brief Checks whether GPU context readiness was marked.
         *
         * @return `true` if GPU context is ready, otherwise `false`.
         */
        [[nodiscard]] bool isGPUReady() const noexcept {
            return runtimeObject().has<GPUContextReadyComponent<RuntimeHandle>>();
        }

        /**
         * @brief Marks the runtime environment as initialized.
         *
         * @return `true` if initialization marker was added, otherwise `false`.
         */
        [[nodiscard]] bool initialize() noexcept {
            if (isInitialized()) {
                assert(false && "RuntimeEnvironment already initialized");
                return false;
            }
            runtimeObject().add<PlatformInitializedComponent<RuntimeHandle>>();
            return true;
        }

        /**
         * @brief Checks whether initialization marker is present.
         *
         * @return `true` if initialized, otherwise `false`.
         */
        [[nodiscard]] bool isInitialized() const noexcept {
            return runtimeObject().has<PlatformInitializedComponent<RuntimeHandle>>();
        }

        /**
         * @brief Checks whether audio backend is ready.
         *
         * @return `true` when audio is considered ready.
         */
        [[nodiscard]] bool isAudioReady() const noexcept {
            return true;
        }

        /**
         * @brief Marks GPU context readiness.
         */
        void setGPUReady() noexcept {
            assert(!isGPUReady() && "GPUContextReadyComponent already set");
            runtimeObject().add<GPUContextReadyComponent<RuntimeHandle>>();
        }

        /**
         * @brief Checks whether all runtime-infrastructure prerequisites are ready.
         *
         * @return `true` if runtime infrastructure is ready, otherwise `false`.
         */
        [[nodiscard]] bool isRuntimeInfrastructureReady() const noexcept {
            return isGPUReady() && isAudioReady();;
        }
            
    };


}