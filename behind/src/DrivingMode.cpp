#include "../include/DrivingMode.h"
#include <cmath>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <chrono>
#include <thread>

DrivingMode::DrivingMode()
    : currentMode(DrivingModeType::NORMAL) {
    
    // Initialize max speed limits as per Operation.md
    maxSpeedLimits[DrivingModeType::NORMAL] = 220.0;      // 220 km/h
    maxSpeedLimits[DrivingModeType::SPORT] = 265.0;       // 265 km/h
    maxSpeedLimits[DrivingModeType::ECO] = 180.0;         // 180 km/h
    maxSpeedLimits[DrivingModeType::SNOW_OFFROAD] = 120.0; // 120 km/h
    
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

bool DrivingMode::changeMode(DrivingModeType newMode, double& currentSpeed) {
    // Nếu tốc độ hiện tại vượt quá giới hạn của chế độ mới
    if (currentSpeed > maxSpeedLimits.at(newMode)) {
        // Hiển thị cảnh báo
        std::cout << "Cannot change mode at high speed. Gradually reducing speed..." << std::endl;
        
        // Giảm tốc độ từ từ 5 km/h mỗi giây
        const double DECREMENT_RATE = 5.0; // 5 km/h mỗi giây
        const double TIME_STEP = 0.1; // Giảm tốc mỗi 0.1 giây
        
        // Giảm tốc độ theo từng bước nhỏ để mượt mà hơn
        while (currentSpeed > maxSpeedLimits.at(newMode)) {
            double speedDecrement = DECREMENT_RATE * TIME_STEP;
            currentSpeed -= speedDecrement;
            
            // Đảm bảo tốc độ không thấp hơn giới hạn của chế độ mới
            currentSpeed = std::max(maxSpeedLimits.at(newMode), currentSpeed);
            
            std::cout << "Speed gradually reducing to " << std::fixed << std::setprecision(1) << currentSpeed 
                      << " km/h to match " << getModeString(newMode) << " mode limit" << std::endl;
            
            // Thêm delay thực tế thay vì giả lập
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(TIME_STEP * 1000)));
        }
        
        std::cout << "Speed reduced to acceptable level for " 
                  << getModeString(newMode) << " mode" << std::endl;
    }
    
    // Thay đổi chế độ
    currentMode = newMode;
    std::cout << "Driving mode changed to: " << getModeString(newMode) << std::endl;
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
