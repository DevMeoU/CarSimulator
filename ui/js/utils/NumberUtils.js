/**
 * Utility functions for rounding numbers in the car simulator
 */

const NumberUtils = {
    /**
     * Round speed value to 1 decimal place
     * @param {number} value Speed value
     * @returns {number} Rounded speed
     */
    roundSpeed(value) {
        return Number(Math.round(value + 'e1') + 'e-1');
    },

    /**
     * Round temperature value to 1 decimal place
     * @param {number} value Temperature value
     * @returns {number} Rounded temperature
     */
    roundTemperature(value) {
        return Number(Math.round(value + 'e1') + 'e-1');
    },

    /**
     * Round battery percentage to whole number
     * @param {number} value Battery percentage
     * @returns {number} Rounded battery percentage
     */
    roundBattery(value) {
        return Math.round(value);
    },

    /**
     * Round engine power/torque to whole number
     * @param {number} value Engine power/torque value
     * @returns {number} Rounded engine value
     */
    roundEngine(value) {
        return Math.round(value);
    },

    /**
     * Round distance to 1 decimal place
     * @param {number} value Distance value
     * @returns {number} Rounded distance
     */
    roundDistance(value) {
        return Number(Math.round(value + 'e1') + 'e-1');
    },

    /**
     * Round pedal position (brake/gas) to percentage
     * @param {number} value Pedal position value
     * @returns {number} Rounded pedal position
     */
    roundPedal(value) {
        return Math.round(value * 100) / 100;
    }
};

export default NumberUtils;