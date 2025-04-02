#include "../include/Engine.h"
#include "../include/DrivingMode.h"
#include <cmath>
#include <sstream>
#include <iomanip>

Engine::Engine() 
    : power(150.0),         // 150 kW as per Database.md
      maxTorque(320.0),     // 320 Nm as per Database.md
      currentPower(0.0),    // No power output when initialized
      temperature(25.0),    // Normal operating temperature
      overheated(false) {
}

Engine::Engine(double power, double maxTorque)
    : power(power),
      maxTorque(maxTorque),
      currentPower(0.0),
      temperature(25.0),
      overheated(false) {
}

Engine::~Engine() {
    // Cleanup if needed
}

// Getters
double Engine::getPower() const {
    return power;
}

double Engine::getMaxTorque() const {
    return maxTorque;
}

double Engine::getCurrentPower() const {
    return currentPower;
}

double Engine::getTemperature() const {
    return temperature;
}

bool Engine::isOverheated() const {
    return overheated;
}

// Setters
void Engine::setPower(double power) {
    this->power = power;
}

void Engine::setMaxTorque(double torque) {
    this->maxTorque = torque;
}

void Engine::setCurrentPower(double power) {
    this->currentPower = power;
}

void Engine::setTemperature(double temp) {
    this->temperature = temp;
    
    // Check for overheating
    if (temperature > 90.0) {
        overheated = true;
    } else if (temperature < 80.0 && overheated) {
        // Reset overheating flag once temperature drops below threshold
        overheated = false;
    }
}

void Engine::start() {
    // Initialize engine state for starting
    currentPower = 0.0;
}

void Engine::stop() {
    // Reset engine state when stopped
    currentPower = 0.0;
}

double Engine::updatePower(double throttlePosition, DrivingModeType mode) {
    // Ensure throttle position is between 0 and 1
    throttlePosition = std::max(0.0, std::min(1.0, throttlePosition));
    
    // Base power calculation
    double requestedPower = power * throttlePosition;
    
    // Apply driving mode factor
    double modeFactor = 1.0;
    switch (mode) {
        case DrivingModeType::SPORT:
            // Sport mode allows full power
            modeFactor = 1.0;
            break;
        case DrivingModeType::ECO:
            // Eco mode limits power to save energy
            modeFactor = 0.7;
            break;
        case DrivingModeType::SNOW_OFFROAD:
            // Snow/Off-road mode limits power for better control
            modeFactor = 0.8;
            break;
        case DrivingModeType::NORMAL:
        default:
            modeFactor = 0.9;  // Normal mode has slight power limitation
            break;
    }
    
    // Apply overheating limitation if necessary
    if (overheated) {
        // Reduce power by 50% when overheated as per Database.md
        modeFactor *= 0.5;
    }
    
    // Calculate actual power output
    currentPower = requestedPower * modeFactor;
    
    // Temperature increases with power output
    temperature += (currentPower / power) * 0.5;
    
    return currentPower;
}

void Engine::updateTemperature(double ambientTemp, double load) {
    // Engine temperature tends to move toward ambient temperature
    // but increases with load (power output)
    double tempDiff = ambientTemp - temperature;
    
    // Temperature change rate depends on the difference and load
    double tempChangeRate = 0.05 * tempDiff + 10.0 * load;
    
    // Update temperature (limited change per update)
    temperature += std::max(-1.0, std::min(3.0, tempChangeRate));
    
    // Check for overheating
    if (temperature > 90.0) {
        overheated = true;
    } else if (temperature < 80.0 && overheated) {
        // Reset overheating flag once temperature drops below threshold
        overheated = false;
    }
}

void Engine::coolDown(double deltaTime) {
    // Simulate cooling system effect
    // More aggressive cooling when temperature is high
    double coolingRate = 0.1;
    if (temperature > 70.0) {
        coolingRate = 0.2;
    }
    if (temperature > 85.0) {
        coolingRate = 0.5;
    }
    
    // Apply cooling
    temperature -= coolingRate * deltaTime;
    
    // Check if no longer overheated
    if (temperature < 80.0 && overheated) {
        overheated = false;
    }
}

void Engine::handleOverheat() {
    if (overheated) {
        // Reduce power output by 50% as per Database.md
        currentPower *= 0.5;
        
        // Activate liquid cooling system (simulated by increasing cooling rate)
        temperature -= 2.0;
    }
}

double Engine::calculateAcceleration(double vehicleWeight) const {
    // F = m * a
    // P = F * v
    // At low speeds, we can approximate using torque
    // a = F / m = T / (m * r)
    // where r is wheel radius (assume 0.3m)
    
    // For simplicity, we'll use a direct calculation based on power-to-weight ratio
    // This is a simplified model for demonstration
    
    // Convert kW to W
    double powerWatts = currentPower * 1000.0;
    
    // Assume vehicle speed of 10 m/s for this calculation
    double assumedSpeed = 10.0;
    
    // Calculate force: P = F * v, so F = P / v
    double force = powerWatts / assumedSpeed;
    
    // Calculate acceleration: a = F / m
    double acceleration = force / vehicleWeight;
    
    // Limit to realistic values
    return std::max(0.0, std::min(10.0, acceleration));
}

std::string Engine::getStatusString() const {
    std::stringstream ss;
    ss << "Engine Status:" << std::endl;
    ss << "  Max Power: " << power << " kW" << std::endl;
    ss << "  Max Torque: " << maxTorque << " Nm" << std::endl;
    ss << "  Current Power: " << std::fixed << std::setprecision(1) << currentPower << " kW" << std::endl;
    ss << "  Temperature: " << std::fixed << std::setprecision(1) << temperature << "°C" << std::endl;
    ss << "  Overheated: " << (overheated ? "Yes" : "No") << std::endl;
    
    return ss.str();
}

void Engine::adjustPerformanceForAltitude(double altitude) {
    // Reduce power output by 5% per 1000m altitude as per documentation
    double altitudeFactor = 1.0 - (altitude / 1000.0) * 0.05;
    currentPower *= std::max(0.7, altitudeFactor);  // Don't reduce below 70% power
}
