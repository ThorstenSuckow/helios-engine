/**
 * @file uniforms.ixx
 * @brief Type tags for strongly typed shader uniform bindings.
 */
module;

export module helios.engine.rendering.shader.types.uniforms;

import helios.engine.rendering.shader.types.UniformSemantics;
import helios.math;

/**
 * @namespace helios::engine::rendering::shader::types
 * @brief Uniform tag types mapping value types to `UniformSemantics`.
 */
export namespace helios::engine::rendering::shader::types {

    /**
     * @brief Tag for the model matrix uniform.
     */
    struct ModelMatrixUniform {
        using value_type = helios::math::mat4f;
        static constexpr UniformSemantics semantics  = UniformSemantics::ModelMatrix;
    };

    /**
     * @brief Tag for the projection matrix uniform.
     */
    struct ProjectionMatrixUniform {
        using value_type = helios::math::mat4f;
        static constexpr UniformSemantics semantics  = UniformSemantics::ProjectionMatrix;
    };

    /**
     * @brief Tag for the view matrix uniform.
     */
    struct ViewMatrixUniform {
        using value_type = helios::math::mat4f;
        static constexpr UniformSemantics semantics  = UniformSemantics::ViewMatrix;
    };

    /**
     * @brief Tag for the material base-color uniform.
     */
    struct MaterialBaseColorUniform {
        using value_type = helios::math::vec4f;
        static constexpr UniformSemantics semantics  = UniformSemantics::MaterialBaseColor;
    };

    /**
     * @brief Tag for the material base-color uniform.
     */
    struct IsInstancedUniform {
        using value_type = bool;
        static constexpr UniformSemantics semantics  = UniformSemantics::IsInstanced;
    };


}