#ifndef FAULT_SIMULATION_H
#define FAULT_SIMULATION_H

#include <string>
#include <vector>

// Forward declarations
class Vehicle;

/**
 * @enum FaultType
 * @brief Represents different types of faults that can occur
 */
enum class FaultType {
    NONE,
    SENSOR_DISCONNECT,
    ENGINE_OVERHEAT,
    BATTERY_FAULT,
    BRAKE_FAILURE
};

/**
 * @class FaultSimulation
 * @brief Simulates various fault scenarios in the vehicle
 */
class FaultSimulation {
private:
    FaultType currentFault;
    bool faultActive;
    
public:
    /**
     * @brief Default constructor
     */
    FaultSimulation();
    
    /**
     * @brief Destructor
     */
    ~FaultSimulation();
    
    // Getters
    FaultType getCurrentFault() const;
    bool isFaultActive() const;
    
    // Setters
    void setCurrentFault(FaultType fault);
    void setFaultActive(bool state);
    
    /**
     * @brief Simulate a specific fault
     * @param fault Type of fault to simulate
     * @return Vector of effects caused by the fault
     */
    void simulateFault(FaultType fault, double severity);
    void resetFaults();
    
    /**
     * @brief Clear current fault
     */
    void clearFault();
    
    /**
     * @brief Handle current fault
     * @param vehicle Reference to the vehicle
     * @return Vector of actions taken to handle the fault
     */
    std::vector<std::string> handleFault(Vehicle& vehicle);
    
    /**
     * @brief Handle sensor disconnect fault
     * @param vehicle Reference to the vehicle
     * @return Vector of actions taken
     */
    std::vector<std::string> handleSensorDisconnect(Vehicle& vehicle);
    
    /**
     * @brief Handle engine overheat fault
     * @param vehicle Reference to the vehicle
     * @return Vector of actions taken
     */
    std::vector<std::string> handleEngineOverheat(Vehicle& vehicle);
    
    /**
     * @brief Handle battery fault
     * @param vehicle Reference to the vehicle
     * @return Vector of actions taken
     */
    std::vector<std::string> handleBatteryFault(Vehicle& vehicle);
    
    /**
     * @brief Handle brake failure
     * @param vehicle Reference to the vehicle
     * @return Vector of actions taken
     */
    std::vector<std::string> handleBrakeFailure(Vehicle& vehicle);
    
    /**
     * @brief Get fault type as string
     * @param fault Fault type
     * @return Fault type string
     */
    static std::string getFaultString(FaultType fault);
    
    /**
     * @brief Get current fault as string
     * @return Current fault string
     */
    std::string getCurrentFaultString() const;
    
    /**
     * @brief Get fault simulation status as string
     * @return Status string
     */
    std::string getStatusString() const;
};

#endif // FAULT_SIMULATION_H
