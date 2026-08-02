/**
 * @file GameObject.ixx
 * @brief Lightweight facade alias for runtime game-object entities.
 */
module;

export module helios.engine.runtime.world.ParticleEntity;

import helios.engine.runtime.world.ParticleEntityManager;
import helios.ecs.Entity;

using namespace helios::ecs;
export namespace helios::engine::runtime::world {

    /**
     * @brief Alias for an entity facade backed by `ParticleEntityManager`.
     */
    using ParticleEntity = Entity<ParticleEntityManager>;

} // namespace helios
