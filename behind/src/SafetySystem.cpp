#include "../include/SafetySystem.h"
#include <cmath>
#include <sstream>
#include <iomanip>

SafetySystem::SafetySystem()
    : absActive(false),
      espActive(false),
      adasActive(false),
      batteryMonitorActive(false),
      airbagCount(6) {  // 6 airbags as per Database.md
}

SafetySystem::~SafetySystem() {
    // Cleanup if needed
}

bool SafetySystem::checkStartConditions(const VehicleData& data) const {
    // Verify minimum battery level and safe temperature range
    return (data.battery > 20.0 &&
            airbagCount > 0);
}

// Getters
bool SafetySystem::isAbsActive() const {
    return absActive;
}

bool SafetySystem::isEspActive() const {
    return espActive;
}

bool SafetySystem::isAdasActive() const {
    return adasActive;
}

bool SafetySystem::isBatteryMonitorActive() const {
    return batteryMonitorActive;
}

int SafetySystem::getAirbagCount() const {
    return airbagCount;
}

// Setters
void SafetySystem::setAbsActive(bool state) {
    absActive = state;
}

void SafetySystem::setEspActive(bool state) {
    espActive = state;
}

void SafetySystem::setAdasActive(bool state) {
    adasActive = state;
}

void SafetySystem::setBatteryMonitorActive(bool state) {
    batteryMonitorActive = state;
}

bool SafetySystem::activateAbs(double speed, double brakeForce) {
    // ABS operates between 5-200 km/h as per Database.md
    if (speed < 5.0 || speed > 200.0) {
        absActive = false;
        return false;
    }
    
    // ABS activates when brake force is high and speed is above threshold
    if (brakeForce > 0.7) {
        // Simulate ABS adjusting brake pressure 100 times/second as per Database.md
        absActive = true;
        return true;
    }
    
    absActive = false;
    return false;
}

bool SafetySystem::activateEsp(double steeringAngle, double slipAngle) {
    // ESP activates when slip angle exceeds 15 degrees as per Database.md
    if (std::abs(slipAngle) > 15.0) {
        espActive = true;
        return true;
    }
    
    // ESP also activates when steering angle changes rapidly
    if (std::abs(steeringAngle) > 45.0) {
        espActive = true;
        return true;
    }
    
    espActive = false;
    return false;
}

bool SafetySystem::monitorBattery(double temperature) {
    batteryMonitorActive = true;
    
    // Check if temperature is within safe limits
    // As per Database.md, warning at 45°C, critical at 60°C
    if (temperature > 45.0) {
        // Temperature is above warning threshold
        if (temperature > 60.0) {
            // Temperature is above critical threshold
            // In a real system, this would trigger emergency procedures
            return false;
        }
        // Temperature is between warning and critical thresholds
        return false;
    }
    
    // Temperature is within safe limits
    return true;
}

bool SafetySystem::activateFcw(double distanceToObstacle, double speed) {
    if (!adasActive) {
        return false;
    }
    
    // Calculate time to collision (TTC) in seconds
    double ttc = (speed > 0) ? (distanceToObstacle / (speed / 3.6)) : 999.0;
    
    // FCW activates when TTC is below threshold
    // Threshold depends on speed
    double threshold = 2.5;  // Base threshold in seconds
    if (speed > 80.0) {
        threshold = 3.0;  // Higher threshold at higher speeds
    }
    
    return (ttc < threshold);
}

bool SafetySystem::activateAeb(double distanceToObstacle, double speed) {
    if (!adasActive) {
        return false;
    }
    
    // Calculate time to collision (TTC) in seconds
    double ttc = (speed > 0) ? (distanceToObstacle / (speed / 3.6)) : 999.0;
    
    // AEB activates when TTC is critically low
    // Threshold depends on speed
    double threshold = 1.5;  // Base threshold in seconds
    if (speed > 80.0) {
        threshold = 2.0;  // Higher threshold at higher speeds
    }
    
    return (ttc < threshold);
}

std::vector<std::string> SafetySystem::enterSafeMode() {
    std::vector<std::string> measures;
    
    // Limit speed to 50 km/h as per Database.md
    measures.push_back("Speed limited to 50 km/h");
    
    // Display warning on dashboard
    measures.push_back("Safe Mode activated - System malfunction detected");
    
    // Activate emergency lights
    measures.push_back("Emergency lights activated");
    
    // Disable non-essential systems
    measures.push_back("Non-essential systems disabled");
    
    return measures;
}

bool SafetySystem::activateEmergencyBrake() {
    // In case of brake failure, activate regenerative braking and electromagnetic emergency brake
    // as per Database.md
    
    // This would trigger maximum regenerative braking
    // and activate the electromagnetic emergency brake
    
    return true;
}

bool SafetySystem::hasBrakeSystemError() const {
    // Check for brake system errors by monitoring ABS status and brake power
    if (absActive && brakePower < 1.0) {
        // ABS is active but brake power is too low
        return true;
    }
    
    // Check if brake power is within normal operating range (0-324 km/h/s)
    if (brakePower < 0.0 || brakePower > 324.0) {
        return true;
    }
    
    return false;
}

bool SafetySystem::hasSystemError() const {
    // Check for any critical system errors
    
    // Check brake system
    if (hasBrakeSystemError()) {
        return true;
    }
    
    // Check ESP system
    if (espActive && !absActive) {
        // ESP is active but ABS is not - potential system conflict
        return true;
    }
    
    // Check ADAS system
    if (adasActive && !batteryMonitorActive) {
        // ADAS requires battery monitoring
        return true;
    }
    
    // Check airbag system
    if (airbagCount <= 0) {
        // No functional airbags
        return true;
    }
    
    return false;
}

std::string SafetySystem::getStatusString() const {
    std::stringstream ss;
    ss << "Safety Systems Status:" << std::endl;
    ss << "  ABS: " << (absActive ? "Active" : "Standby") << std::endl;
    ss << "  ESP: " << (espActive ? "Active" : "Standby") << std::endl;
    ss << "  ADAS: " << (adasActive ? "Active" : "Standby") << std::endl;
    ss << "  Battery Monitor: " << (batteryMonitorActive ? "Active" : "Standby") << std::endl;
    ss << "  Airbags: " << airbagCount << " installed" << std::endl;
    
    return ss.str();
}
