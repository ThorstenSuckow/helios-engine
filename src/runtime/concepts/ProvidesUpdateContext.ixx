/**
* @file ProvidesUpdateContext.ixx
 * @brief Concept for types exposing UpdateContext.
 */
module;


#include <concepts>


export module helios.engine.runtime.concepts:ProvidesUpdateContext;

export namespace helios::engine::runtime::concepts {


    template<typename T, typename TUpdateContext>
    concept ProvidesUpdateContext = requires(T& t) {
        {t.updateContext()} -> std::same_as<TUpdateContext&>;
    };


}