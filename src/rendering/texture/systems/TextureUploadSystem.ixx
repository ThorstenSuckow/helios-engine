/**
 * @file TextureUploadSystem.ixx
 * @brief Collects active texture resources and queues texture upload commands.
 */
module;

#include <vector>
#include "helios-engine-config.h"

export module helios.engine.rendering.texture.systems:TextureUploadSystem;

import helios.engine.rendering.texture.types;

import helios.engine.rendering.texture.concepts;
import helios.engine.rendering.texture.commands;
import helios.engine.rendering.texture.components;

import helios.engine.runtime.messaging.command.NullCommandBuffer;
import helios.engine.runtime.messaging.command.concepts.IsCommandBufferLike;
import helios.engine.runtime.world.tags.SystemRole;
import helios.engine.runtime.world.UpdateContext;
import helios.ecs;

using namespace helios::engine::runtime::messaging::command;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::world::tags;
using namespace helios::engine::runtime::messaging::command::concepts;
using namespace helios::ecs::components;
export namespace helios::engine::rendering::texture::systems {


    template<typename THandle = texture::types::TextureHandle,
        typename TCommandBuffer = NullCommandBuffer>
    requires texture::concepts::IsTextureHandle<THandle> && IsCommandBufferLike<TCommandBuffer>
    class TextureUploadSystem {

        std::vector<THandle> textureHandles_;

        size_t capacity_;

    public:

        using EngineRoleTag = TypedSystemRole;
        using CommandBuffer_type = TCommandBuffer;

        explicit TextureUploadSystem(size_t capacity = TEXTURE_INITIAL_STORAGE_CAPACITY) : capacity_(capacity) {
            textureHandles_.reserve(capacity);
        }

        /**
         * @brief Collects texture handles and queues one batch upload command.
         *
         * @param updateContext Frame update context.
         */
        void update(UpdateContext& updateContext, TCommandBuffer& cmdBuffer) noexcept {

            for (auto [entity, textureSource] : updateContext.view<
                THandle,
                texture::components::TextureSourceComponent<THandle>
            >().withActive()) {
                textureHandles_.push_back(entity.handle());
            }

            cmdBuffer.template add<texture::commands::TextureBatchUploadCommand<THandle>>(std::move(textureHandles_));

            textureHandles_.clear();
            textureHandles_.reserve(capacity_);
        }

    };



}