/**
 * @file UniformMapping.ixx
 * @brief Type that maps a uniform semantic to a shader uniform name.
 */
module;

#include <string>

export module helios.engine.rendering.shader.types.UniformMapping;

import helios.engine.rendering.shader.types.UniformSemantics;

export namespace helios::engine::rendering::shader::types {

    /**
     * @brief Mapping entry from `UniformSemantics` to a concrete uniform name.
     */
    struct UniformMapping {

        /**
         * @brief Semantic key used by runtime systems.
         */
        UniformSemantics semantics;

        /**
         * @brief Uniform variable name expected by the shader program.
         */
        std::string name;

    };

}