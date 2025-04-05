#ifndef VEHICLE_H_UPDATED
#define VEHICLE_H_UPDATED

#include "Battery.h"
#include "Engine.h"
#include "DrivingMode.h"
#include "SafetySystem.h"
#include "EnvironmentalCondition.h"
#include "FaultSimulation.h"
#include "Sensor.h"
#include "Display.h"
#include "VehicleData.h"
#include <memory>
#include <string>
#include <atomic>

/**
 * @class Vehicle
 * @brief Main class representing the electric vehicle
 */
class Vehicle {
private:
    Battery battery;
    Engine engine;
    DrivingMode drivingMode;
    SafetySystem safetySystem;
    std::shared_ptr<VehicleData> vehicleData;
    EnvironmentalCondition environment;
    FaultSimulation faultSim;
    Sensor sensor;
    Display display;
    
    static double speed;                // Current speed in km/h
    static double distanceTraveled;     // Total distance traveled in km
    static double brakePressTime;       // Time brake has been pressed in seconds
    static bool doorLocked;             // Whether doors are locked
    static bool seatbeltOn;             // Whether seatbelt is fastened
    static bool engineRunning;          // Whether engine is running
    static bool brakeActive;            // Whether brake is currently active
    static bool acceleratorActive;      // Whether accelerator is currently active
    static bool leftSignalOn;           // Whether left turn signal is on
    static bool rightSignalOn;          // Whether right turn signal is on

public:
    /**
     * @brief Constructor
     */
    Vehicle();
    
    /**
     * @brief Destructor
     */
    ~Vehicle();
    
    /**
     * @brief Update vehicle state
     * @param deltaTime Time elapsed since last update in seconds
     */
    void update(double deltaTime);
    
    /**
     * @brief Start the engine
     * @return true if engine started successfully, false otherwise
     */
    bool startEngine();
    
    /**
     * @brief Stop the engine
     */
    void stopEngine();
    
    /**
     * @brief Press brake for specified time
     * @param seconds Time to press brake in seconds
     */
    void pressBrake(double seconds);
    
    /**
     * @brief Apply brake with specified intensity
     * @param intensity Brake intensity (0-1)
     */
    void brake(double intensity);
    
    /**
     * @brief Accelerate with specified intensity
     * @param intensity Acceleration intensity (0-1)
     */
    void accelerate(double intensity);

    /**
     * @brief Decelerate with specified intensity
     * @param intensity Deceleration intensity (0-1)
     */
    void decelerate(double intensity);
    
    /**
     * @brief Change driving mode
     * @param mode New driving mode
     * @return true if mode changed successfully, false otherwise
     */
    bool changeDrivingMode(DrivingModeType mode);
    
    /**
     * @brief Get current maximum speed based on driving mode
     * @return Maximum speed in km/h
     */
    double getCurrentMaxSpeed() const;
    
    /**
     * @brief Get current speed
     * @return Current speed in km/h
     */
    double getSpeed() const;
    
    /**
     * @brief Get total distance traveled
     * @return Distance in km
     */
    double getDistanceTraveled() const;
    
    /**
     * @brief Get brake press time
     * @return Time in seconds
     */
    double getBrakePressTime() const;
    
    /**
     * @brief Check if doors are locked
     * @return true if locked, false otherwise
     */
    bool isDoorLocked() const;
    
    /**
     * @brief Set door lock state
     * @param locked New lock state
     */
    void setDoorLocked(bool locked);
    
    /**
     * @brief Check if seatbelt is on
     * @return true if on, false otherwise
     */
    bool isSeatbeltOn() const;
    
    /**
     * @brief Set seatbelt state
     * @param on New seatbelt state
     */
    void setSeatbeltOn(bool on);
    
    /**
     * @brief Check if engine is running
     * @return true if running, false otherwise
     */
    bool isEngineRunning() const;
    
    /**
     * @brief Check if brake is active
     * @return true if active, false otherwise
     */
    bool isBrakeActive() const;
    
    /**
     * @brief Check if accelerator is active
     * @return true if active, false otherwise
     */
    bool isAcceleratorActive() const;
    
    /**
     * @brief Check if left turn signal is on
     * @return true if on, false otherwise
     */
    bool isLeftSignalOn() const;
    
    /**
     * @brief Set left turn signal state
     * @param on New signal state
     */
    void setLeftSignalOn(bool on);
    
    /**
     * @brief Check if right turn signal is on
     * @return true if on, false otherwise
     */
    bool isRightSignalOn() const;
    
    /**
     * @brief Set right turn signal state
     * @param on New signal state
     */
    void setRightSignalOn(bool on);
    
    /**
     * @brief Set vehicle gear
     * @param gear Gear to set ("P", "R", "N", "D")
     */
    void setGear(const std::string& gear);
    
    /**
     * @brief Set parking brake state
     * @param on Whether parking brake is engaged
     */
    void setParking(bool on);
    
    /**
     * @brief Update vehicle environment parameters
     * @param temperature Ambient temperature in Celsius
     * @param humidity Relative humidity percentage
     * @param windSpeed Wind speed in km/h
     * @param altitude Altitude in meters
     */
    void updateEnvironment(double temperature, double humidity, double windSpeed, double altitude);
    
    /**
     * @brief Get battery reference
     * @return Reference to battery
     */
    Battery& getBattery();
    
    /**
     * @brief Get engine reference
     * @return Reference to engine
     */
    Engine& getEngine();
    
    /**
     * @brief Get driving mode reference
     * @return Reference to driving mode
     */
    DrivingMode& getDrivingMode();
    
    /**
     * @brief Get safety system reference
     * @return Reference to safety system
     */
    SafetySystem& getSafetySystem();
    
    /**
     * @brief Get environmental condition reference
     * @return Reference to environmental condition
     */
    EnvironmentalCondition& getEnvironment();
    
    /**
     * @brief Get fault simulation reference
     * @return Reference to fault simulation
     */
    FaultSimulation& getFaultSim();
    
    /**
     * @brief Get sensor reference
     * @return Reference to sensor
     */
    Sensor& getSensor();
    
    /**
     * @brief Get display reference
     * @return Reference to display
     */
    Display& getDisplay();
    
    /**
     * @brief Get current battery load
     * @return Battery load percentage (0-100)
     */
    double getBatteryLoad() const;
    
    /**
     * @brief Get vehicle status as string
     * @return Status string
     */
    /**
     * @brief Get vehicle status as string
     * @return Status string
     */
    std::string getStatusString() const;
    void simulateFault(FaultType faultType, double severity);
    void resetFaults();
    std::string getWeatherCondition() const;
    void emergencyStop();
    bool isEmergencyStopped() const;
    
    // Additional methods from Vehicle.cpp

};

#endif // VEHICLE_H_UPDATED
