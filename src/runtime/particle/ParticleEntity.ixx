/**
 * @file GameObject.ixx
 * @brief Lightweight facade alias for runtime game-object entities.
 */
module;

export module helios.engine.runtime.particle.ParticleEntity;

import helios.engine.runtime.particle.ParticleEntityManager;
import helios.ecs.Entity;

using namespace helios::ecs;
export namespace helios::engine::runtime::particle {

    /**
     * @brief Alias for an entity facade backed by `ParticleEntityManager`.
     */
    using ParticleEntity = Entity<ParticleEntityManager>;

} // namespace helios
