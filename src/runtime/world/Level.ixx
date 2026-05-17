/**
 * @file Level.ixx
 * @brief Defines the Level class representing a game level or stage.
 */
module;

export module helios.engine.runtime.world.Level;

import helios.math.types;
import helios.engine.core.units.Unit;



export namespace helios::engine::runtime::world {

    /**
     * @brief Represents a game level containing the scene graph and world boundaries.
     *
     * @details The Level class acts as a container for the scene hierarchy (via a root node)
     * and defines the spatial limits of the game world. It handles unit conversion for
     * boundaries to ensure consistent internal representation (meters).
     */
    class Level {


        /**
         * @brief The spatial boundaries of the level in engine units (meters).
         */
        helios::math::aabbf bounds_;
    public:




        /**
         * @brief Sets the spatial boundaries of the level.
         *
         * @details Converts the provided bounds from the specified unit to the engine's
         * standard unit (meters) before storing them.
         *
         * @param bounds The axis-aligned bounding box defining the level limits.
         * @param unit The unit of measurement for the provided bounds (defaults to Meter).
         */
        void setBounds(
            const helios::math::aabbf& bounds,
            const  helios::engine::core::units::Unit unit = helios::engine::core::units::Unit::Meter) noexcept {

            bounds_ = helios::engine::core::units::from(bounds, unit);

        }


        /**
         * @brief Retrieves the level boundaries in engine units (meters).
         *
         * @return The axis-aligned bounding box of the level.
         */
        [[nodiscard]] const helios::math::aabbf& bounds() const noexcept {
            return bounds_;
        }

    };

}


