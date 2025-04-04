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
      currentCharge(0.8),   // Start with 80% charge (more realistic)
      temperature(30.0),    // Normal operating temperature (adjusted for realistic conditions)
      range(450.0),         // 450 km as per Database.md
      charging(false) {
}

Battery::Battery(double capacity, double voltage, double range)
    : capacity(capacity),
      voltage(voltage),
      currentCharge(1.0),
      temperature(25.0),
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
    currentCharge -= (speed * 0.01 + temperature * 0.005) * deltaTime;
    currentCharge = std::max(0.0, std::min(currentCharge, capacity));
}

double Battery::getCurrentLoad() const {
    return currentCharge * 100;
}

double Battery::getChargeLevel() const {
    return currentCharge;
}

double Battery::getChargePercentage() const {
    return (currentCharge / capacity) * 100.0;
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
    // Battery temperature tends to move toward ambient temperature
    // but increases with load (discharge/charge rate)
    double tempDiff = ambientTemp - temperature;
    
    // Temperature change rate depends on the difference and current load
    double load = getCurrentCharge(); // Use current charge as load factor
    double tempChangeRate = 0.1 * tempDiff + (load * 5.0);
    
    // Update temperature (limited change per update)
    temperature += std::max(-2.0, std::min(2.0, tempChangeRate));
    
    // Ensure temperature stays within realistic bounds
    temperature = std::max(-20.0, std::min(80.0, temperature));
}

bool Battery::checkTemperature() const {
    // Check if temperature is within safe limits
    // As per Database.md, warning at 45°C, critical at 60°C
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
    
    // Based on Database.md:
    // Fast charging: 0-80% in 30 minutes
    // AC charging: 7.4 kW (8 hours for full charge)
    
    if (fastCharging) {
        // Fast charging rate is not linear - it slows down after 80%
        if (targetLevel <= 0.8) {
            // Simple linear calculation for 0-80%
            return (chargeNeeded / 0.8) * 30.0;
        } else {
            // Time to reach 80%
            double timeTo80Percent = (0.8 - currentCharge) / 0.8 * 30.0;
            
            // Remaining charge needed after 80%
            double remainingCharge = targetLevel - 0.8;
            
            // Charging slows down significantly after 80%
            double timeAfter80Percent = remainingCharge / 0.2 * 60.0;
            
            return std::max(0.0, timeTo80Percent) + timeAfter80Percent;
        }
    } else {
        // AC charging - linear rate
        // 8 hours for full charge (0-100%)
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
