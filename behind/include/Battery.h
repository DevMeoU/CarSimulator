#ifndef BATTERY_H
#define BATTERY_H

#include <string>

// Forward declarations
class EnvironmentalCondition;
enum class DrivingModeType;

/**
 * @class Battery
 * @brief Represents the lithium-ion battery system of the electric vehicle
 */
class Battery {
private:
    double capacity;        // kWh
    double voltage;         // V
    double currentCharge;   // Current charge level (0-1)
    double temperature;     // Battery temperature in Celsius
    double range;           // km
    bool charging;          // Charging flag
    
public:
    /**
     * @brief Default constructor
     */
    Battery();
    
    /**
     * @brief Parameterized constructor
     * @param capacity Battery capacity in kWh
     * @param voltage Battery voltage in V
     * @param range Maximum range in km
     */
    Battery(double capacity, double voltage, double range);
    
    /**
     * @brief Destructor
     */
    ~Battery();
    
    // Getters
    double getCapacity() const;
    double getVoltage() const;
    double getCurrentCharge() const;
    double getTemperature() const;
    double getRange() const;
    bool isCharging() const;
    
    // Setters
    void setCapacity(double capacity);
    void setVoltage(double voltage);
    void setCurrentCharge(double charge);
    void setTemperature(double temp);
    void setRange(double range);
    void setCharging(bool state);
    
    /**
     * @brief Charge the battery
     * @param amount Amount to charge (0-1)
     * @return Actual amount charged
     */
    double charge(double amount);
    
    /**
     * @brief Discharge the battery
     * @param amount Amount to discharge (0-1)
     * @return Actual amount discharged
     */
    double discharge(double amount);
    
    /**
     * @brief Calculate remaining range based on current charge, driving mode and environmental conditions
     * @param mode Current driving mode
     * @param env Current environmental conditions
     * @return Remaining range in km
     */
    double calculateRemainingRange(DrivingModeType mode, const EnvironmentalCondition& env) const;
    
    /**
     * @brief Update battery temperature based on ambient temperature and load
     * @param ambientTemp Ambient temperature in Celsius
     * @param load Current load factor (0-1)
     */
    void updateTemperature(double ambientTemp, double load);
    
    /**
     * @brief Check if battery temperature is within safe limits
     * @return true if temperature is safe, false otherwise
     */
    bool checkTemperature() const;
    
    /**
     * @brief Disconnect high voltage in case of emergency
     */
    void disconnectHighVoltage();
    
    /**
     * @brief Isolate faulty battery module
     */
    void isolateFaultyModule();
    
    /**
     * @brief Calculate charging time from current level to target level
     * @param targetLevel Target charge level (0-1)
     * @param fastCharging Whether to use fast charging
     * @return Charging time in minutes
     */
    double calculateChargingTime(double targetLevel, bool fastCharging) const;
    
    /**
     * @brief Get battery status as string
     * @return Status string
     */
    std::string getStatusString() const;
};

#endif // BATTERY_H
