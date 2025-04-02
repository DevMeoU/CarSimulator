#ifndef ENGINE_H
#define ENGINE_H

#include <string>

// Forward declarations
enum class DrivingModeType;

/**
 * @class Engine
 * @brief Represents the electric motor of the vehicle
 */
class Engine {
private:
    double power;           // kW
    double maxTorque;       // Nm
    double currentPower;    // Current power output
    double temperature;     // Engine temperature in Celsius
    bool overheated;        // Overheating flag
    double throttle;

public:
    double getThrottle() const { return throttle; }
    void setThrottle(double value) { throttle = value; }
    /**
     * @brief Default constructor
     */
    Engine();
    
    /**
     * @brief Parameterized constructor
     * @param power Maximum power in kW
     * @param maxTorque Maximum torque in Nm
     */
    Engine(double power, double maxTorque);
    
    /**
     * @brief Destructor
     */
    ~Engine();
    
    // Getters
    double getPower() const;
    double getMaxTorque() const;
    double getCurrentPower() const;
    double getTemperature() const;
    bool isOverheated() const;
    
    // Setters
    void setPower(double power);
    void setMaxTorque(double torque);
    void setCurrentPower(double power);
    void setTemperature(double temp);
    
    /**
     * @brief Start the engine
     */
    void start();
    
    /**
     * @brief Stop the engine
     */
    void stop();
    
    /**
     * @brief Update power output based on throttle position and driving mode
     * @param throttlePosition Throttle position (0-1)
     * @param mode Current driving mode
     * @return Actual power output
     */
    double updatePower(double throttlePosition, DrivingModeType mode);
    
    /**
     * @brief Update engine temperature based on ambient temperature and load
     * @param ambientTemp Ambient temperature in Celsius
     * @param load Current load factor (0-1)
     */
    void updateTemperature(double ambientTemp, double load);
    
    /**
     * @brief Cool down engine over time
     * @param deltaTime Time elapsed in seconds
     */
    void coolDown(double deltaTime);
    
    /**
     * @brief Handle overheating situation
     */
    void handleOverheat();
    
    /**
     * @brief Calculate acceleration based on current power and vehicle weight
     * @param vehicleWeight Vehicle weight in kg
     * @return Acceleration in m/s²
     */
    double calculateAcceleration(double vehicleWeight) const;
    
    /**
     * @brief Get engine status as string
     * @return Status string
     */
    std::string getStatusString() const;
    
    /**
     * @brief Adjust engine performance based on altitude
     * @param altitude Current altitude in meters
     */
    void adjustPerformanceForAltitude(double altitude);
};

#endif // ENGINE_H
