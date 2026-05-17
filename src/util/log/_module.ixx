/**
 * @file _module.ixx
 * @brief Module exports for logging.
 */

/**
 * @brief Logging system for debugging and diagnostics.
 *
 * @details This module provides logging functionality including logger instances,
 * log management, and configurable output sinks for flexible log destinations.
 */
export module helios.engine.util.log;

export import helios.engine.util.log.LogSink;
export import helios.engine.util.log.ConsoleSink;
export import helios.engine.util.log.Logger;
export import helios.engine.util.log.LogManager;