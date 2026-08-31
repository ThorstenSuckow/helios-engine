/**
 * @file TextureUploadSystem.ixx
 * @brief Collects active texture resources and queues texture upload commands.
 */
module;

#include <vector>
#include "helios-engine-config.h"

export module helios.engine.rendering.texture.systems:TextureUploadSystem;

import helios.engine.rendering.texture.commands;
import helios.engine.rendering.texture.components;

import helios.ecs.command.NullCommandBuffer;
import helios.ecs.command.types;
import helios.ecs.common.concepts;

import helios.engine.runtime.gameloop.types;

import helios.ecs;
import helios.ecs.entity.EntityWorld;

using namespace helios::ecs;


using namespace helios::ecs::command::concepts;
using namespace helios::ecs::command;
using namespace helios::ecs::components;
export namespace helios::engine::rendering::texture::systems {

    template<typename THandle>
    class TextureUploadSystem {

        using EntityWorld = ecs::entity::EntityWorld;

        std::vector<THandle> textureHandles_;

        size_t capacity_;

    public:

        using CommandBuffer = ecs::command::TypedCommandBuffer<texture::commands::TextureBatchUploadCommand<THandle>>;


        explicit TextureUploadSystem(size_t capacity = TEXTURE_INITIAL_STORAGE_CAPACITY) : capacity_(capacity) {
            textureHandles_.reserve(capacity);
        }

        /**
         * @brief Collects texture handles and queues one batch upload command.
         *
         * @param ecsWorld Frame ECS world.
         * @return true if the update was successful.
         */
        void update(EntityWorld& ecsWorld, CommandBuffer& cmdBuffer) noexcept {

            for (auto [entity, textureSource] : ecsWorld.view<
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