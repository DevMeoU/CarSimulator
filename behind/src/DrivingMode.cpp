#include "../include/DrivingMode.h"
#include <cmath>
#include <sstream>
#include <iomanip>

DrivingMode::DrivingMode()
    : currentMode(DrivingModeType::NORMAL) {
    
    // Initialize max speed limits as per Operation.md
    maxSpeedLimits[DrivingModeType::NORMAL] = 150.0;      // 150 km/h
    maxSpeedLimits[DrivingModeType::SPORT] = 180.0;       // 180 km/h
    maxSpeedLimits[DrivingModeType::ECO] = 120.0;         // 120 km/h
    maxSpeedLimits[DrivingModeType::SNOW_OFFROAD] = 80.0; // 80 km/h
    
    // Initialize power factors
    powerFactors[DrivingModeType::NORMAL] = 0.9;      // 90% of max power
    powerFactors[DrivingModeType::SPORT] = 1.0;       // 100% of max power
    powerFactors[DrivingModeType::ECO] = 0.7;         // 70% of max power
    powerFactors[DrivingModeType::SNOW_OFFROAD] = 0.8; // 80% of max power
    
    // Initialize regenerative braking factors
    regenerativeBrakingFactors[DrivingModeType::NORMAL] = 0.7;      // 70% of max regen
    regenerativeBrakingFactors[DrivingModeType::SPORT] = 0.5;       // 50% of max regen (less aggressive)
    regenerativeBrakingFactors[DrivingModeType::ECO] = 1.0;         // 100% of max regen (most aggressive)
    regenerativeBrakingFactors[DrivingModeType::SNOW_OFFROAD] = 0.6; // 60% of max regen
}

DrivingMode::~DrivingMode() {
    // Cleanup if needed
}

// Getters
DrivingModeType DrivingMode::getCurrentMode() const {
    return currentMode;
}

double DrivingMode::getMaxSpeedLimit() const {
    return maxSpeedLimits.at(currentMode);
}

double DrivingMode::getMaxSpeedLimit(DrivingModeType mode) const {
    return maxSpeedLimits.at(mode);
}

double DrivingMode::getPowerFactor() const {
    return powerFactors.at(currentMode);
}

double DrivingMode::getRegenerativeBrakingFactor() const {
    return regenerativeBrakingFactors.at(currentMode);
}

// Setters
void DrivingMode::setCurrentMode(DrivingModeType mode) {
    currentMode = mode;
}

bool DrivingMode::changeMode(DrivingModeType newMode, double currentSpeed) {
    // Check if current speed exceeds the max speed limit of the new mode
    if (currentSpeed > maxSpeedLimits.at(newMode)) {
        // Cannot change mode at high speed as per Operation.md
        return false;
    }
    
    // Change mode
    currentMode = newMode;
    return true;
}

double DrivingMode::adjustPower(double basePower) const {
    // Apply power factor based on current mode
    return basePower * powerFactors.at(currentMode);
}

double DrivingMode::adjustRegenerativeBraking(double baseRegeneration) const {
    // Apply regenerative braking factor based on current mode
    return baseRegeneration * regenerativeBrakingFactors.at(currentMode);
}

double DrivingMode::calculateAccelerationTime() const {
    // Calculate 0-100 km/h acceleration time based on mode
    // As per Database.md, Sport mode has 5.2 seconds for 0-100 km/h
    double baseTime = 5.2;  // Sport mode time
    
    switch (currentMode) {
        case DrivingModeType::SPORT:
            return baseTime;  // 5.2 seconds
        case DrivingModeType::NORMAL:
            return baseTime * 1.2;  // 20% slower than Sport
        case DrivingModeType::ECO:
            return baseTime * 1.5;  // 50% slower than Sport
        case DrivingModeType::SNOW_OFFROAD:
            return baseTime * 1.8;  // 80% slower than Sport
        default:
            return baseTime;
    }
}

std::string DrivingMode::getModeString(DrivingModeType mode) {
    switch (mode) {
        case DrivingModeType::NORMAL:
            return "Normal";
        case DrivingModeType::SPORT:
            return "Sport";
        case DrivingModeType::ECO:
            return "Eco";
        case DrivingModeType::SNOW_OFFROAD:
            return "Snow/Off-road";
        default:
            return "Unknown";
    }
}

std::string DrivingMode::getCurrentModeString() const {
    return getModeString(currentMode);
}

std::string DrivingMode::getStatusString() const {
    std::stringstream ss;
    ss << "Driving Mode Status:" << std::endl;
    ss << "  Current Mode: " << getCurrentModeString() << std::endl;
    ss << "  Max Speed Limit: " << std::fixed << std::setprecision(1) << getMaxSpeedLimit() << " km/h" << std::endl;
    ss << "  Power Factor: " << std::fixed << std::setprecision(2) << getPowerFactor() << std::endl;
    ss << "  Regenerative Braking Factor: " << std::fixed << std::setprecision(2) << getRegenerativeBrakingFactor() << std::endl;
    ss << "  0-100 km/h Time: " << std::fixed << std::setprecision(1) << calculateAccelerationTime() << " seconds" << std::endl;
    
    return ss.str();
}
