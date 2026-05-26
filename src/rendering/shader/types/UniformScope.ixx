/**
 * @file UniformScope.ixx
 * @brief Marker types defining uniform update scope for shader bindings.
 */
module;

export module helios.engine.rendering.shader.types.UniformScope;

/**
 * @brief Scope marker types used to categorize uniform lifetime.
 */
export namespace helios::engine::rendering::shader::types::UniformScope {

    /**
     * @brief Uniform value is stable for an entire render pass.
     */
    struct Pass{};

    /**
     * @brief Uniform value can change per draw call.
     */
    struct Draw{};

}