/**
 * @file _module.ixx
 * @brief Input handling module exports.
 */

/**
 * @brief Input handling module for user input management.
 *
 * @details This module provides input management functionality including adapters
 * for different input sources and managers for processing user input. Includes
 * type definitions for various input events and states.
 */
export module helios.engine.input;

export import helios.engine.input.types;
export import helios.engine.input.gamepad;
export import helios.engine.input.InputAdapter;
export import helios.engine.input.InputManager;
export import helios.engine.input.InputSnapshot;
