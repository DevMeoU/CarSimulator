#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <functional>
#include <chrono>
#include <thread>
#include <map>

/**
 * @namespace Utils
 * @brief Contains utility functions for the electric vehicle simulation
 */
namespace Utils {
    /**
     * @brief Convert kilometers per hour to meters per second
     * @param kmh Speed in kilometers per hour
     * @return Speed in meters per second
     */
    double kmhToMs(double kmh);
    
    /**
     * @brief Convert meters per second to kilometers per hour
     * @param ms Speed in meters per second
     * @return Speed in kilometers per hour
     */
    double msToKmh(double ms);
    
    /**
     * @brief Calculate power in watts from force and speed
     * @param force Force in newtons
     * @param speed Speed in meters per second
     * @return Power in watts
     */
    double calculatePower(double force, double speed);
    
    /**
     * @brief Calculate force from mass and acceleration
     * @param mass Mass in kilograms
     * @param acceleration Acceleration in meters per second squared
     * @return Force in newtons
     */
    double calculateForce(double mass, double acceleration);
    
    /**
     * @brief Calculate energy consumption in kWh
     * @param power Power in kilowatts
     * @param hours Time in hours
     * @return Energy in kilowatt-hours
     */
    double calculateEnergy(double power, double hours);
    
    /**
     * @brief Calculate range based on battery capacity and energy consumption
     * @param batteryCapacity Battery capacity in kilowatt-hours
     * @param energyConsumption Energy consumption in kilowatt-hours per kilometer
     * @return Range in kilometers
     */
    double calculateRange(double batteryCapacity, double energyConsumption);
    
    /**
     * @brief Calculate braking distance
     * @param speed Speed in kilometers per hour
     * @param frictionCoefficient Friction coefficient
     * @return Braking distance in meters
     */
    double calculateBrakingDistance(double speed, double frictionCoefficient);
    
    /**
     * @brief Calculate time to collision
     * @param distance Distance to obstacle in meters
     * @param speed Speed in kilometers per hour
     * @return Time to collision in seconds
     */
    double calculateTimeToCollision(double distance, double speed);
    
    /**
     * @brief Sleep for specified milliseconds
     * @param ms Time in milliseconds
     */
    void sleep(int ms);
    
    /**
     * @brief Run a function at specified interval
     * @param func Function to run
     * @param intervalMs Interval in milliseconds
     * @param durationMs Total duration in milliseconds
     */
    void runAtInterval(std::function<void()> func, int intervalMs, int durationMs);
    
    /**
     * @brief Format time as string
     * @param seconds Time in seconds
     * @return Formatted time string (MM:SS)
     */
    std::string formatTime(double seconds);
    
    /**
     * @brief Clear console screen
     */
    void clearScreen();

    /**
     * @brief Pause execution until user presses enter
     */
    void pauseExecution();

     
    /**
     * @brief Print progress bar
     * @param progress Progress value (0-1)
     * @param width Width of progress bar
     */
    void printProgressBar(double progress, int width = 50);
    
    /**
     * @brief Get user input with validation
     * @param prompt Prompt message
     * @param validator Validation function
     * @return Valid user input
     */
    std::string getValidatedInput(const std::string& prompt, 
                                 std::function<bool(const std::string&)> validator);
    
    /**
     * @brief Parse command line arguments
     * @param argc Argument count
     * @param argv Argument values
     * @return Map of argument names to values
     */
    std::map<std::string, std::string> parseArgs(int argc, char* argv[]);
}

#endif // UTILS_H
