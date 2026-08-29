/**
 * @file RenderDataResolver.ixx
 * @brief Type erased wrapper for concrete RenderDataResolver implementations.
 */
module;

#include <memory>
#include <optional>
#include <span>

export module helios.engine.rendering.RenderDataResolver;

import helios.ecs;

import helios.engine.rendering.common.types;

export namespace helios::engine::rendering {

    class RenderDataResolver {

        using EntityHandleValue = helios::ecs::common::types::EntityHandleValue;
        using EcsDataContainer = helios::ecs::common::container::EcsDataContainer;
        using EcsDataContainerFunctionInvoker = helios::ecs::common::container::EcsDataContainerFunctionInvoker;
        using RenderData = engine::rendering::common::types::RenderData;


        class Concept {

            public:
            virtual ~Concept() = default;

            virtual std::optional<RenderData> resolveRenderTargetData(EcsDataContainer& ecsDataContainer, EntityHandleValue renderTargetHandleValue) noexcept = 0;
            virtual std::optional<RenderData> resolveViewportData(EcsDataContainer& ecsDataContainer, EntityHandleValue viewportHandleValue) noexcept = 0;
            virtual std::optional<RenderData> resolveMaterialData(EcsDataContainer& ecsDataContainer, EntityHandleValue materialHandleValue) noexcept = 0;
            virtual std::optional<RenderData> resolveMeshData(EcsDataContainer& ecsDataContainer, EntityHandleValue meshHandleValue) noexcept = 0;
            virtual std::optional<RenderData> resolveTextureData(EcsDataContainer& ecsDataContainer, EntityHandleValue textureHandleValue) noexcept = 0;
            virtual std::optional<RenderData> resolveShaderData(EcsDataContainer& ecsDataContainer, EntityHandleValue shaderHandleValue) noexcept = 0;



        };

        template<typename TConcreteDataResolver>
        class Model final: public Concept {

            using RenderTargetHandleType = TConcreteDataResolver::RenderTargetHandle;
            using MaterialHandleType = TConcreteDataResolver::MaterialHandle;
            using MeshHandleType = TConcreteDataResolver::MeshHandle;
            using TextureHandleType = TConcreteDataResolver::TextureHandle;
            using ShaderHandleType = TConcreteDataResolver::ShaderHandle;
            using ViewportHandleType = TConcreteDataResolver::ViewportHandle;


            TConcreteDataResolver dataResolver_;

            public:

            explicit Model(TConcreteDataResolver&& dataResolver) :
            dataResolver_(std::move(dataResolver))
            {}

            std::optional<RenderData> resolveRenderTargetData(EcsDataContainer& ecsDataContainer, EntityHandleValue renderTargetHandleValue) noexcept override {
                auto handle = renderTargetHandleValue.get<RenderTargetHandleType>();

                auto data = EcsDataContainerFunctionInvoker::invoke<&TConcreteDataResolver::resolveRenderTargetData>(
                    dataResolver_,
                    ecsDataContainer,
                    handle
                );

                if (data) {
                    return RenderData(std::move(*data));
                }

                return std::nullopt;
            }

            std::optional<RenderData> resolveViewportData(EcsDataContainer& ecsDataContainer, EntityHandleValue viewportHandleValue) noexcept override {
                auto handle = viewportHandleValue.get<ViewportHandleType>();
                auto data = EcsDataContainerFunctionInvoker::invoke<&TConcreteDataResolver::resolveViewportData>(
                    dataResolver_,
                    ecsDataContainer,
                    handle
                );

                if (data) {
                    return RenderData(std::move(*data));
                }

                return std::nullopt;
            }

            std::optional<RenderData> resolveMaterialData(EcsDataContainer& ecsDataContainer, EntityHandleValue materialHandleValue) noexcept override {
                auto handle = materialHandleValue.get<MaterialHandleType>();
                auto data = EcsDataContainerFunctionInvoker::invoke<&TConcreteDataResolver::resolveMaterialData>(
                    dataResolver_,
                    ecsDataContainer,
                    handle
                );

                if (data) {
                    return RenderData(std::move(*data));
                }
                return std::nullopt;
            }

            std::optional<RenderData> resolveMeshData(EcsDataContainer& ecsDataContainer, EntityHandleValue meshHandleValue) noexcept override {
                auto handle = meshHandleValue.get<MeshHandleType>();
                auto data = EcsDataContainerFunctionInvoker::invoke<&TConcreteDataResolver::resolveMeshData>(
                    dataResolver_,
                    ecsDataContainer,
                    handle
                );

                if (data) {
                    return RenderData(std::move(*data));
                }

                return std::nullopt;
            }

            std::optional<RenderData> resolveTextureData(EcsDataContainer& ecsDataContainer, EntityHandleValue textureHandleValue) noexcept override {
                auto handle = textureHandleValue.get<TextureHandleType>();
                auto data = EcsDataContainerFunctionInvoker::invoke<&TConcreteDataResolver::resolveTextureData>(
                    dataResolver_,
                    ecsDataContainer,
                    handle
                );
                if (data) {
                    return RenderData(std::move(*data));
                }
                return std::nullopt;
            }

            std::optional<RenderData> resolveShaderData(EcsDataContainer& ecsDataContainer, EntityHandleValue shaderHandleValue) noexcept override {
                auto handle = shaderHandleValue.get<ShaderHandleType>();
                auto data = EcsDataContainerFunctionInvoker::invoke<&TConcreteDataResolver::resolveShaderData>(
                    dataResolver_,
                    ecsDataContainer,
                    handle
                );
                if (data) {
                    return RenderData(std::move(*data));
                }
                return std::nullopt;
            }

        };

        std::unique_ptr<Concept> model_;

        public:

        template<typename TConcreteDataResolver>
        explicit RenderDataResolver(TConcreteDataResolver&& dataResolver) :
        model_(std::make_unique<Model<TConcreteDataResolver>>(std::move(dataResolver)))
        {}

        std::optional<RenderData> resolveRenderTargetData(EcsDataContainer& ecsDataContainer, EntityHandleValue renderTargetHandleValue) noexcept {
            return model_->resolveRenderTargetData(ecsDataContainer, renderTargetHandleValue);
        }
        std::optional<RenderData> resolveViewportData(EcsDataContainer& ecsDataContainer, EntityHandleValue viewportHandleValue) noexcept {
            return model_->resolveViewportData(ecsDataContainer, viewportHandleValue);
        }

        std::optional<RenderData> resolveMaterialData(EcsDataContainer& ecsDataContainer, EntityHandleValue materialHandleValue) noexcept {
            return model_->resolveMaterialData(ecsDataContainer, materialHandleValue);
        }

        std::optional<RenderData> resolveMeshData(EcsDataContainer& ecsDataContainer, EntityHandleValue meshHandleValue) noexcept {
            return model_->resolveMeshData(ecsDataContainer, meshHandleValue);
        }

        std::optional<RenderData> resolveTextureData(EcsDataContainer& ecsDataContainer, EntityHandleValue textureHandleValue) noexcept {
            return model_->resolveTextureData(ecsDataContainer, textureHandleValue);
        }

        std::optional<RenderData> resolveShaderData(EcsDataContainer& ecsDataContainer, EntityHandleValue shaderHandleValue) noexcept {
            return model_->resolveShaderData(ecsDataContainer, shaderHandleValue);
        }
    };



};