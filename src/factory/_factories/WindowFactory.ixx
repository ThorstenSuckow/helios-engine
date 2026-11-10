module;

#include <cstdint>
#include <string>

export module helios.engine.factory:WindowFactory;

import helios.ecs.entity.EntityWorld;
import :Window;

export namespace helios::engine::factory {

    class WindowFactory {

        using EntityWorld = helios::ecs::entity::EntityWorld;

        template<typename TRenderHandles>
        using RenderTargetEntity = ecs::entity::Entity<ecs::entity::EntityManager<typename TRenderHandles::RenderTargetHandle>>;

        EntityWorld& ecsWorld_;

    public:

        WindowFactory(const WindowFactory&) = delete;
        WindowFactory& operator=(const WindowFactory&) = delete;
        WindowFactory(WindowFactory&&) = delete;
        WindowFactory& operator=(WindowFactory&&) = delete;

        explicit WindowFactory(EntityWorld& ecsWorld) : ecsWorld_(ecsWorld) {}

        template<typename TWindowHandle, typename TRenderHandles>
        Window<TWindowHandle, TRenderHandles> createWindow(
            const std::string& title, const std::uint32_t width, const std::uint32_t height,
            RenderTargetEntity<TRenderHandles> renderTargetEntity
        ) {
            auto entity = ecsWorld_.add<TWindowHandle>();
            entity.setActive(true);

            return Window<TWindowHandle, TRenderHandles>(
                ecsWorld_, entity, title, width, height, renderTargetEntity
            );
        }

    };

}