/**
 * @file Key.ixx
 * @brief Key code definitions used across input adapters.
 */
module;

export module helios.engine.input.types.Key;

export namespace helios::engine::input::types {

    /**
     * @brief Enumerates common keyboard keys.
     *
     * This enumeration provides identifiers for various keyboard keys.
     * It's intended to be a platform-agnostic representation of physical keys.
     *
     * Implementing APIs are advised to properly map their implementation against
     * this enumeration.
     */
    enum Key {

        ESC,

        TILDE,

        /**
         * @brief Number of entries in this enumeration.
         */
        size_
    };

}