#ifndef DRIVING_MODE_H
#define DRIVING_MODE_H

#include <map>
#include <string>

/**
 * @enum DrivingModeType
 * @brief Represents different driving modes of the vehicle
 */
enum class DrivingModeType {
    NORMAL,
    SPORT,
    ECO,
    SNOW_OFFROAD
};

/**
 * @class DrivingMode
 * @brief Manages different driving modes and their parameters
 */
class DrivingMode {
private:
    DrivingModeType currentMode;
    std::map<DrivingModeType, double> maxSpeedLimits;
    std::map<DrivingModeType, double> powerFactors;
    std::map<DrivingModeType, double> regenerativeBrakingFactors;
    
public:
    /**
     * @brief Default constructor
     */
    DrivingMode();
    
    /**
     * @brief Destructor
     */
    ~DrivingMode();
    
    // Getters
    DrivingModeType getCurrentMode() const;
    double getMaxSpeedLimit() const;
    double getMaxSpeedLimit(DrivingModeType mode) const;
    double getPowerFactor() const;
    double getRegenerativeBrakingFactor() const;
    
    // Setters
    void setCurrentMode(DrivingModeType mode);
    
    /**
     * @brief Change driving mode
     * @param newMode New driving mode
     * @param currentSpeed Current vehicle speed
     * @return true if mode change was successful, false otherwise
     */
    bool changeMode(DrivingModeType newMode, double currentSpeed);
    
    /**
     * @brief Adjust power based on current driving mode
     * @param basePower Base power value
     * @return Adjusted power value
     */
    double adjustPower(double basePower) const;
    
    /**
     * @brief Adjust regenerative braking based on current driving mode
     * @param baseRegeneration Base regenerative braking value
     * @return Adjusted regenerative braking value
     */
    double adjustRegenerativeBraking(double baseRegeneration) const;
    
    /**
     * @brief Calculate acceleration time (0-100 km/h)
     * @return Acceleration time in seconds
     */
    double calculateAccelerationTime() const;
    
    /**
     * @brief Get mode name as string
     * @param mode Driving mode
     * @return Mode name string
     */
    static std::string getModeString(DrivingModeType mode);
    
    /**
     * @brief Get current mode name as string
     * @return Current mode name string
     */
    std::string getCurrentModeString() const;
    
    /**
     * @brief Get driving mode status as string
     * @return Status string
     */
    std::string getStatusString() const;
};

#endif // DRIVING_MODE_H
