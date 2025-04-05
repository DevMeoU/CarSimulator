#include "../include/Battery.h"
#include "../include/EnvironmentalCondition.h"
#include "../include/DrivingMode.h"
#include <cmath>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <chrono>

Battery::Battery() 
    : capacity(75.0),       // 75 kWh as per Database.md
      voltage(400.0),       // 400 V as per Database.md
      currentCharge(0.8),   // Start with 80% charge (0.8 = 80%, value range: 0.0-1.0)
      temperature(35.0),    // Normal operating temperature (below warning threshold of 45°C)
      range(450.0),         // 450 km as per Database.md
      charging(false) {
}

Battery::Battery(double capacity, double voltage, double range)
    : capacity(capacity),
      voltage(voltage),
      currentCharge(0.8),   // Consistent with default constructor
      temperature(35.0),    // Consistent with default constructor
      range(range),
      charging(false) {
}

Battery::~Battery() {
    // Cleanup if needed
}

// Getters
double Battery::getCapacity() const {
    return capacity;
}

double Battery::getVoltage() const {
    return voltage;
}

double Battery::getCurrentCharge() const {
    return currentCharge;
}

void Battery::updateCharge(double deltaTime, double speed) {
    // Tính toán mức tiêu hao pin dựa trên:
    // - Tốc độ: 0.0001667 kWh/km (tính theo % capacity) để đạt 450km với pin 75kWh
    // - Nhiệt độ: 0.00002 kWh/°C (tính theo % capacity)
    // - Thời gian: deltaTime để cập nhật theo thời gian thực
    double speedConsumption = (speed * 0.0001667) / capacity;
    double tempConsumption = (temperature * 0.00002) / capacity;
    currentCharge -= (speedConsumption + tempConsumption) * deltaTime;
    
    // Đảm bảo giá trị nằm trong khoảng hợp lệ (0-1)
    currentCharge = std::max(0.0, std::min(1.0, currentCharge));
}

double Battery::getCurrentLoad() const {
    return getChargePercentage();
}

double Battery::getChargeLevel() const {
    return currentCharge;
}

double Battery::getChargePercentage() const {
    return currentCharge * 100.0;
}

double Battery::getTemperature() const {
    return temperature;
}

double Battery::getRange() const {
    return range;
}

bool Battery::isCharging() const {
    return charging;
}

// Setters
void Battery::setCapacity(double capacity) {
    this->capacity = capacity;
}

void Battery::setVoltage(double voltage) {
    this->voltage = voltage;
}

void Battery::setCurrentCharge(double charge) {
    // Ensure charge is between 0 and 1
    this->currentCharge = std::max(0.0, std::min(1.0, charge));
}

void Battery::setTemperature(double temp) {
    this->temperature = temp;
}

void Battery::setRange(double range) {
    this->range = range;
}

void Battery::setCharging(bool state) {
    this->charging = state;
}

double Battery::charge(double amount) {
    if (!charging) {
        return 0.0;
    }
    
    // Calculate how much we can actually charge
    double maxCharge = 1.0 - currentCharge;
    double actualCharge = std::min(amount, maxCharge);
    
    // Update current charge
    currentCharge += actualCharge;
    
    // Temperature increases slightly during charging
    temperature += actualCharge * 5.0;
    
    return actualCharge;
}

double Battery::discharge(double amount) {
    if (charging) {
        return 0.0;
    }
    
    // Calculate how much we can actually discharge
    double actualDischarge = std::min(amount, currentCharge);
    
    // Update current charge
    currentCharge -= actualDischarge;
    
    // Temperature increases slightly during discharge
    temperature += actualDischarge * 3.0;
    
    return actualDischarge;
}

double Battery::calculateRemainingRange(DrivingModeType mode, const EnvironmentalCondition& env) const {
    // Base range at full charge
    double baseRange = range * currentCharge;
    
    // Apply driving mode factor
    double modeFactor = 1.0;
    switch (mode) {
        case DrivingModeType::ECO:
            // Eco mode increases range by 15% as per Database.md
            modeFactor = 1.15;
            break;
        case DrivingModeType::SPORT:
            // Sport mode decreases range due to higher power consumption
            modeFactor = 0.8;
            break;
        case DrivingModeType::SNOW_OFFROAD:
            // Snow/Off-road mode decreases range due to higher power consumption
            modeFactor = 0.85;
            break;
        case DrivingModeType::NORMAL:
        default:
            modeFactor = 1.0;
            break;
    }
    
    // Apply environmental factors
    double tempFactor = 1.0;
    double altitudeFactor = 1.0;
    double loadFactor = 1.0;
    double terrainFactor = 1.0;
    double weatherFactor = 1.0;
    
    // Temperature impact as per Database.md
    if (env.getTemperature() < 0) {
        // Below 0°C: Reduce range by 20-30%
        tempFactor = 0.75;
    } else if (env.getTemperature() > 35) {
        // Above 35°C: Reduce range by 10%
        tempFactor = 0.9;
    }
    
    // Altitude impact as per Database.md
    // 5% reduction per 1000m
    altitudeFactor = 1.0 - (env.getAltitude() / 1000.0) * 0.05;
    
    // Load impact as per Database.md
    // 5-8% reduction per 100kg
    loadFactor = 1.0 - (env.getLoad() / 100.0) * 0.065;
    
    // Terrain impact as per Database.md
    if (env.getSlope() > 0) {
        // 25% increase in energy consumption for 10% slope
        terrainFactor = 1.0 - (env.getSlope() / 10.0) * 0.25;
    }
    
    if (env.getRoughness() > 0.5) {
        // Rough terrain reduces range
        terrainFactor *= 0.8;
    }
    
    // Weather impact
    switch (env.getWeather()) {
        case WeatherType::RAIN:
            weatherFactor = 0.95;  // Rain slightly reduces range
            break;
        case WeatherType::SNOW:
            weatherFactor = 0.85;  // Snow significantly reduces range
            break;
        case WeatherType::FOG:
            weatherFactor = 0.97;  // Fog slightly reduces range
            break;
        case WeatherType::CLEAR:
        default:
            weatherFactor = 1.0;
            break;
    }
    
    // Combine all factors
    double totalFactor = modeFactor * tempFactor * altitudeFactor * loadFactor * terrainFactor * weatherFactor;
    
    return baseRange * totalFactor;
}

void Battery::updateTemperature(double ambientTemp, std::chrono::system_clock::time_point timestamp) {
    // Cập nhật nhiệt độ pin dựa trên:
    // - Nhiệt độ môi trường
    // - Tải sử dụng pin
    // - Thời gian từ lần cập nhật trước
    double tempDiff = ambientTemp - temperature;
    
    // Tốc độ thay đổi nhiệt độ phụ thuộc vào:
    // - Chênh lệch nhiệt độ với môi trường (0.1°C/s)
    // - Tải sử dụng (tăng 5°C khi tải đầy)
    double load = getCurrentLoad() / 100.0; // Chuyển đổi phần trăm thành tỷ lệ
    double tempChangeRate = 0.1 * tempDiff + (load * 5.0);
    
    // Giới hạn tốc độ thay đổi nhiệt độ (-2°C đến +2°C mỗi lần cập nhật)
    temperature += std::max(-2.0, std::min(2.0, tempChangeRate));
    
    // Đảm bảo nhiệt độ nằm trong giới hạn an toàn
    // Theo Database.md: Cảnh báo ở 45°C, ngắt ở 60°C
    temperature = std::max(-20.0, std::min(60.0, temperature));
}

bool Battery::checkTemperature() const {
    // Kiểm tra nhiệt độ pin có nằm trong giới hạn an toàn
    // Theo Database.md:
    // - Cảnh báo khi nhiệt độ > 45°C
    // - Ngắt sạc khi nhiệt độ > 60°C
    return temperature < 45.0;
}

void Battery::disconnectHighVoltage() {
    // Simulate disconnecting high voltage in case of emergency
    charging = false;
    // Additional safety measures would be implemented here
}

void Battery::isolateFaultyModule() {
    // Simulate isolating a faulty battery module
    // In a real system, this would involve hardware controls
    capacity *= 0.9;  // Reduce capacity by 10% to simulate isolation
    range *= 0.9;     // Reduce range accordingly
}

double Battery::calculateChargingTime(double targetLevel, bool fastCharging) const {
    double chargeNeeded = targetLevel - currentCharge;
    
    if (chargeNeeded <= 0) {
        return 0.0;
    }
    
    // Tính thời gian sạc dựa trên Database.md:
    // - Sạc nhanh DC: 0-80% trong 30 phút
    // - Sạc AC: 7.4 kW (8 giờ để sạc đầy)
    
    if (fastCharging) {
        // Tốc độ sạc nhanh không tuyến tính - chậm lại sau 80%
        if (targetLevel <= 0.8) {
            // Tính tuyến tính cho 0-80%
            // Công thức: (Phần trăm cần sạc / 80%) * 30 phút
            return (chargeNeeded / 0.8) * 30.0;
        } else {
            // Thời gian để đạt 80%
            double timeTo80Percent = std::max(0.0, (0.8 - currentCharge)) / 0.8 * 30.0;
            
            // Phần còn lại cần sạc sau 80%
            double remainingCharge = targetLevel - 0.8;
            
            // Sạc chậm hơn đáng kể sau 80% (gấp đôi thời gian)
            // 20% còn lại mất 60 phút
            double timeAfter80Percent = remainingCharge / 0.2 * 60.0;
            
            return timeTo80Percent + timeAfter80Percent;
        }
    } else {
        // Sạc AC - tốc độ tuyến tính
        // 8 giờ để sạc đầy (0-100%)
        // Công thức: (Phần trăm cần sạc) * 8 giờ * 60 phút
        return chargeNeeded * 8.0 * 60.0;
    }
}

std::string Battery::getStatusString() const {
    std::stringstream ss;
    ss << "Battery Status:" << std::endl;
    ss << "  Capacity: " << capacity << " kWh" << std::endl;
    ss << "  Voltage: " << voltage << " V" << std::endl;
    ss << "  Charge: " << std::fixed << std::setprecision(1) << (currentCharge * 100.0) << "%" << std::endl;
    ss << "  Temperature: " << std::fixed << std::setprecision(1) << temperature << "°C" << std::endl;
    ss << "  Estimated Range: " << std::fixed << std::setprecision(1) << (range * currentCharge) << " km" << std::endl;
    ss << "  Charging: " << (charging ? "Yes" : "No") << std::endl;
    
    return ss.str();
}

bool Battery::hasChargingError() const {
    // Check for charging-related errors
    if (charging) {
        // Critical temperature while charging
        if (temperature > 60.0) return true;
        
        // Attempting to charge a full battery
        if (currentCharge >= 1.0) return true;
        
        // Temperature rising too quickly during charging
        if (temperature > 45.0) return true;
    }
    
    // Check for extremely low charge level
    if (currentCharge < 0.05) return true;
    
    return false;
}
