/**
 * @file SpawnPolicyRegistry.ixx
 * @brief Registry mapping spawn policy types to their implementations.
 */
module;

export module helios.engine.runtime.spawning.SpawnPolicyRegistry;


import helios.engine.runtime.spawning.SpawnPolicy;
import helios.engine.runtime.spawning.types;
import helios.engine.core.container.ConceptModelRegistry;


export namespace helios::engine::runtime::spawning {

    /**
     * @brief `ConceptModelRegistry` specialisation that stores `SpawnPolicy` instances
     *        keyed by `SpawnPolicyTypeId`.
     *
     * @tparam TEmitterHandle  Handle type of the emitting entity.
     * @tparam TSpawnHandle    Handle type of the spawned entities.
     */
    template<typename TEmitterHandle, typename TSpawnHandle>
    using SpawnPolicyRegistry = helios::engine::core::container::ConceptModelRegistry<
        SpawnPolicy<TEmitterHandle, TSpawnHandle>, types::SpawnPolicyTypeId<TEmitterHandle, TSpawnHandle>
    >;

}