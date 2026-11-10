module;

#include <cstdint>
#include <string>

export module helios.engine.factory:Factory;

import helios.ecs.entity.EntityWorld;

import :RenderTargetFactory;
import :Scene;
import :SceneFactory;
import :Window;
import :WindowFactory;

export namespace helios::engine::factory {

    /**
     * @brief Factory for creating engine-level resources and entities.
     *
     * @details The Factory provides methods to create engine-level resources
     * (e.g. render targets, viewports, cameras) and entities with their associated
     * components. It abstracts away the details of entity creation and component
     * initialization, providing a convenient interface for constructing complex
     * objects in the engine.
     */
    template<typename TRenderHandles>
    class Factory {

        using EntityWorld = helios::ecs::entity::EntityWorld;

        EntityWorld& ecsWorld_;
        WindowFactory windowFactory_;
        RenderTargetFactory renderTargetFactory_;
        SceneFactory sceneFactory_;

    public:

        Factory(const Factory&) = delete;
        Factory& operator=(const Factory&) = delete;
        Factory(Factory&&) = delete;
        Factory& operator=(Factory&&) = delete;

        explicit Factory (EntityWorld& ecsWorld) :
        ecsWorld_(ecsWorld),
        windowFactory_(ecsWorld),
        renderTargetFactory_(ecsWorld),
        sceneFactory_(ecsWorld) {}

        template<typename TWindowHandle>
        Window<TWindowHandle, TRenderHandles> createWindow(
            const std::string& title, const std::uint32_t width, const std::uint32_t height
        ) {
            return windowFactory_.createWindow<TWindowHandle, TRenderHandles>(
                title, width, height,
                renderTargetFactory_.createRenderTarget<TRenderHandles>().entity()
            );
        }

        Scene<TRenderHandles> createScene() {
            return sceneFactory_.createScene<TRenderHandles>();
        }

    };

}