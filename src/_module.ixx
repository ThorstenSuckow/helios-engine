/**
 * @file _module.ixx
 * @brief Root module aggregating all helios subsystems.
 */

/**
 * @brief The helios game engine module.
 *
 * @details This is the root module that aggregates all subsystems of the helios framework,
 * including core functionality, mathematical operations, rendering, scene management,
 * input handling, event system, and application infrastructure. Importing this module
 * provides access to the entire helios engine API.
 */
export module helios;

export import helios.engine.core;
export import helios.math;
export import helios.engine.rendering;
export import helios.engine.scene;
export import helios.engine.input;
export import helios.engine.util;
export import helios.engine.state;
export import helios.engine.spatial;
export import helios.engine.runtime;
export import helios.engine.platform;
export import helios.engine.tooling;

export import helios.bootstrap;

