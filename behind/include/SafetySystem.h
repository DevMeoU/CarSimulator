#ifndef SAFETY_SYSTEM_H
#define SAFETY_SYSTEM_H

#include <string>
#include <vector>

/**
 * @class SafetySystem
 * @brief Manages various safety systems of the vehicle
 */
class SafetySystem {
private:
    bool absActive;
    bool espActive;
    bool adasActive;
    bool batteryMonitorActive;
    int airbagCount;
    double brakePower;
    
public:
    double getBrakePower() const { return brakePower; }
    void applyBrake(double intensity) { brakePower = intensity * 100; }
    /**
     * @brief Default constructor
     */
    SafetySystem();
    
    /**
     * @brief Destructor
     */
    ~SafetySystem();
    
    // Getters
    bool isAbsActive() const;
    bool isEspActive() const;
    bool isAdasActive() const;
    bool isBatteryMonitorActive() const;
    int getAirbagCount() const;
    
    // Setters
    void setAbsActive(bool state);
    void setEspActive(bool state);
    void setAdasActive(bool state);
    void setBatteryMonitorActive(bool state);
    
    /**
     * @brief Activate ABS (Anti-lock Braking System)
     * @param speed Current vehicle speed in km/h
     * @param brakeForce Brake force applied (0-1)
     * @return true if ABS was activated, false otherwise
     */
    bool activateAbs(double speed, double brakeForce);
    
    /**
     * @brief Activate ESP (Electronic Stability Program)
     * @param steeringAngle Current steering angle in degrees
     * @param slipAngle Current slip angle in degrees
     * @return true if ESP was activated, false otherwise
     */
    bool activateEsp(double steeringAngle, double slipAngle);
    
    /**
     * @brief Monitor battery temperature
     * @param temperature Battery temperature in Celsius
     * @return true if temperature is within safe limits, false otherwise
     */
    bool monitorBattery(double temperature);
    
    /**
     * @brief Activate FCW (Forward Collision Warning)
     * @param distanceToObstacle Distance to obstacle in meters
     * @param speed Current vehicle speed in km/h
     * @return true if warning was activated, false otherwise
     */
    bool activateFcw(double distanceToObstacle, double speed);
    
    /**
     * @brief Activate AEB (Autonomous Emergency Braking)
     * @param distanceToObstacle Distance to obstacle in meters
     * @param speed Current vehicle speed in km/h
     * @return true if emergency braking was activated, false otherwise
     */
    bool activateAeb(double distanceToObstacle, double speed);
    
    /**
     * @brief Enter safe mode in case of system failure
     * @return Vector of safety measures taken
     */
    std::vector<std::string> enterSafeMode();
    
    /**
     * @brief Activate emergency brake
     * @return true if emergency brake was activated, false otherwise
     */
    bool activateEmergencyBrake();
    
    /**
     * @brief Get safety system status as string
     * @return Status string
     */
    std::string getStatusString() const;
};

#endif // SAFETY_SYSTEM_H
