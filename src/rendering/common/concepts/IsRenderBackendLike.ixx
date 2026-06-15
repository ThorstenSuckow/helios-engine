/**
 * @file IsRenderBackendLike.ixx
 * @brief Concept constraining render backend interfaces.
 */
module;

#include <concepts>
#include <span>

export module helios.engine.rendering.common.concepts.IsRenderBackendLike;

import helios.math.types;

import helios.engine.rendering.common.types.RenderPassContext;
import helios.engine.util.Colors;

import helios.engine.scene.types.SceneMemberRenderContext;

import helios.engine.rendering.renderTarget.types;
import helios.engine.rendering.viewport.types;
import helios.engine.rendering.shader.types;
import helios.engine.rendering.common.types;
import helios.engine.rendering.material.types;
import helios.engine.rendering.mesh.types;

using namespace helios::math;
using namespace helios::engine::rendering::renderTarget::types;
using namespace helios::engine::rendering::viewport::types;
using namespace helios::engine::rendering::common::types;
using namespace helios::engine::scene::types;

using namespace helios::engine::rendering::shader::types;
using namespace helios::engine::rendering::mesh::types;
using namespace helios::engine::rendering::material::types;
using namespace helios::engine::rendering::viewport::types;
using namespace helios::engine::rendering::renderTarget::types;

export namespace helios::engine::rendering::common::concepts {

    /**
     * @brief Constrains types that provide the rendering backend API surface.
     *
     * @tparam T Backend candidate type.
     * @tparam THandle Scene member handle type used by `doRender`.
     */
    template<typename T, typename THandle>
    concept IsRenderBackendLike = requires(
            T& t,
            const T& ct,
            const RenderTargetHandle renderTargetHandle,
            const ViewportHandle viewportHandle,
            const ShaderHandle shaderHandle,
            const MaterialHandle materialHandle,
            const MeshHandle meshHandle,
            std::span<const SceneMemberRenderContext<THandle>> sceneMemberRenderContexts,
            std::span<const InstanceData<THandle>> instanceData
           )
        {
        {t.beginRenderTargetBatch(renderTargetHandle)}->std::same_as<void>;
        {t.endRenderTargetBatch(renderTargetHandle)}->std::same_as<void>;
        {t.beginViewportBatch(viewportHandle)}->std::same_as<void>;
        {t.endViewportBatch(viewportHandle)}->std::same_as<void>;
        {t.beginShaderBatch(shaderHandle)}->std::same_as<void>;
        {t.endShaderBatch(shaderHandle)}->std::same_as<void>;
        {t.beginMaterialBatch(materialHandle)}->std::same_as<void>;
        {t.endMaterialBatch(materialHandle)}->std::same_as<void>;
        {t.beginMeshBatch(meshHandle)}->std::same_as<void>;
        {t.endMeshBatch(meshHandle)}->std::same_as<void>;
        {t.renderBatch(sceneMemberRenderContexts)}->std::same_as<void>;
        {t.renderBatch(instanceData)}->std::same_as<void>;
    };




}