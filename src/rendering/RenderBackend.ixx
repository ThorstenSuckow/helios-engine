/**
 * @file RenderBackend.ixx
 * @brief Type erased wrapper for concrete RenderBackend implementations.
 */
module;

#include <memory>
#include <span>

export module helios.engine.rendering.RenderBackend;

import helios.ecs;

import helios.core.common.traits;

import helios.engine.rendering.common.types;

export namespace helios::engine::rendering {

    class RenderBackend {

        using EntityHandleValue = helios::ecs::common::types::EntityHandleValue;
        using EcsDataContainer = helios::ecs::common::container::EcsDataContainer;
        using DrawData = helios::engine::rendering::common::types::DrawData;
        using InstanceData = helios::engine::rendering::common::types::InstanceData;
        using RenderData = helios::engine::rendering::common::types::RenderData;


        class Concept {

            public:
            virtual ~Concept() = default;

            virtual void beginRenderTargetBatch(const RenderData& renderData) noexcept = 0;
            virtual void endRenderTargetBatch() noexcept = 0;

            virtual void beginViewportBatch(const RenderData& renderData) noexcept = 0;
            virtual void endViewportBatch() noexcept = 0;

            virtual void beginShaderBatch(const RenderData& renderData) noexcept = 0;
            virtual void endShaderBatch() noexcept = 0;

            virtual void beginTextureBatch(const RenderData& renderData) noexcept = 0;
            virtual void endTextureBatch() noexcept = 0;

            virtual void beginMaterialBatch(const RenderData& renderData) noexcept = 0;
            virtual void endMaterialBatch() noexcept = 0;

            virtual void beginMeshBatch(const RenderData& renderData) noexcept = 0;
            virtual void endMeshBatch() noexcept = 0;

            virtual void renderBatch(std::span<const DrawData> drawData) noexcept = 0;

            virtual void renderBatch(std::span<const InstanceData> instanceData) noexcept = 0;

            virtual void configureWindowCreationHints() noexcept = 0;

            [[nodiscard]]virtual bool finalizeSetup() noexcept = 0;

            [[nodiscard]] virtual bool isInitialized() const noexcept = 0;


        };

        template<typename TConcreteBackend>
        class Model final: public Concept {

            TConcreteBackend concreteBackend_;

            public:

            explicit Model(TConcreteBackend&& concreteBackend)
            : concreteBackend_(std::move(concreteBackend)) {}

            void beginRenderTargetBatch(const RenderData& renderData) noexcept override {
                using traits = helios::core::common::traits::FunctionSignatureTraits<decltype(&TConcreteBackend::beginRenderTargetBatch)>;
                using TargetType = typename traits::template UnqualifiedArgumentType<0>;
                concreteBackend_.beginRenderTargetBatch(renderData.get<TargetType>());
            }
            void endRenderTargetBatch() noexcept override {
                concreteBackend_.endRenderTargetBatch();
            }
            
            void beginViewportBatch(const RenderData& renderData) noexcept override {
                using traits = helios::core::common::traits::FunctionSignatureTraits<decltype(&TConcreteBackend::beginViewportBatch)>;
                using TargetType = typename traits::template UnqualifiedArgumentType<0>;
                concreteBackend_.beginViewportBatch(renderData.get<TargetType>());
            }
            void endViewportBatch() noexcept override {
                concreteBackend_.endViewportBatch();
            }
            
            void beginShaderBatch(const RenderData& renderData) noexcept override {
                using traits = helios::core::common::traits::FunctionSignatureTraits<decltype(&TConcreteBackend::beginShaderBatch)>;
                using TargetType = typename traits::template UnqualifiedArgumentType<0>;
                concreteBackend_.beginShaderBatch(renderData.get<TargetType>());
            }
            void endShaderBatch() noexcept override {
                concreteBackend_.endShaderBatch();
            }
            
            void beginTextureBatch(const RenderData& renderData) noexcept override {
                using traits = helios::core::common::traits::FunctionSignatureTraits<decltype(&TConcreteBackend::beginTextureBatch)>;
                using TargetType = typename traits::template UnqualifiedArgumentType<0>;
                concreteBackend_.beginTextureBatch(renderData.get<TargetType>());
            }
            void endTextureBatch() noexcept override {
                concreteBackend_.endTextureBatch();
            }
            
            void beginMaterialBatch(const RenderData& renderData) noexcept override {
                using traits = helios::core::common::traits::FunctionSignatureTraits<decltype(&TConcreteBackend::beginMaterialBatch)>;
                using TargetType = typename traits::template UnqualifiedArgumentType<0>;
                concreteBackend_.beginMaterialBatch(renderData.get<TargetType>());
            }
            void endMaterialBatch() noexcept override {
                concreteBackend_.endMaterialBatch();
            }
            
            void beginMeshBatch(const RenderData& renderData) noexcept override {
                using traits = helios::core::common::traits::FunctionSignatureTraits<decltype(&TConcreteBackend::beginMeshBatch)>;
                using TargetType = typename traits::template UnqualifiedArgumentType<0>;
                concreteBackend_.beginMeshBatch(renderData.get<TargetType>());
            }
            void endMeshBatch() noexcept override {
                concreteBackend_.endMeshBatch();
            }
            
            void renderBatch(std::span<const DrawData> drawData) noexcept override {
                concreteBackend_.renderBatch(drawData);
            }

            void renderBatch(std::span<const InstanceData> instanceData) noexcept override {
                concreteBackend_.renderBatch(instanceData);
            }

            void configureWindowCreationHints() noexcept override {
                concreteBackend_.configureWindowCreationHints();
            }

            [[nodiscard]] bool finalizeSetup() noexcept override {
                return concreteBackend_.finalizeSetup();
            }

            [[nodiscard]] bool isInitialized() const noexcept override {
                return concreteBackend_.isInitialized();
            }
            

        };

        std::unique_ptr<Concept> model_;

        public:

        template<typename TConcreteBackend>
        explicit RenderBackend(TConcreteBackend&& renderBackend) :
        model_(std::make_unique<Model<TConcreteBackend>>(std::move(renderBackend)))
        {}

        void beginRenderTargetBatch(const RenderData& renderData) noexcept {
            model_->beginRenderTargetBatch(renderData);
        }
        void endRenderTargetBatch() noexcept {
            model_->endRenderTargetBatch();
        }

        void beginViewportBatch(const RenderData& renderData) noexcept {
            model_->beginViewportBatch(renderData);
        }
        void endViewportBatch() noexcept {
            model_->endViewportBatch();
        }

        void beginShaderBatch(const RenderData& renderData) noexcept {
            model_->beginShaderBatch(renderData);
        }
        void endShaderBatch() noexcept {
            model_->endShaderBatch();
        }

        void beginTextureBatch(const RenderData& renderData) noexcept {
            model_->beginTextureBatch(renderData);
        }
        void endTextureBatch() noexcept {
            model_->endTextureBatch();
        }

        void beginMaterialBatch(const RenderData& renderData) noexcept {
            model_->beginMaterialBatch(renderData);
        }
        void endMaterialBatch() noexcept {
            model_->endMaterialBatch();
        }

        void beginMeshBatch(const RenderData& renderData) noexcept {
            model_->beginMeshBatch(renderData);
        }
        void endMeshBatch() noexcept {
            model_->endMeshBatch();
        }

        void renderBatch(std::span<const DrawData> drawData) {
            model_->renderBatch(drawData);
        }

        void renderBatch(std::span<const InstanceData> instanceData) {
            model_->renderBatch(instanceData);
        }

        [[nodiscard]] bool finalizeSetup() noexcept {
            return model_->finalizeSetup();
        }
        void configureWindowCreationHints() noexcept {
            model_->configureWindowCreationHints();
        }

        [[nodiscard]] bool isInitialized() const noexcept {
            return model_->isInitialized();
        }
    };



};