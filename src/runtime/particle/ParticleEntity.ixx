/**
 * @file GameObject.ixx
 * @brief Lightweight facade alias for runtime game-object entities.
 */
module;

export module helios.engine.runtime.particle.ParticleEntity;

import helios.engine.runtime.particle.types;
import helios.ecs;

using namespace helios::ecs;
export namespace helios::engine::runtime::particle {

    using ParticleEntity = Entity<EntityManager<types::ParticleHandle>>;

} // namespace helios
