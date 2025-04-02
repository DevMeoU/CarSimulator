#include "../include/FaultSimulation.h"
#include "../include/Vehicle.h"
#include <sstream>
#include <iomanip>

FaultSimulation::FaultSimulation()
    : currentFaults{FaultType::NONE},
      faultActive(false) {
}



FaultSimulation::~FaultSimulation() {
    // Cleanup if needed
}



// Getters
std::vector<FaultType> FaultSimulation::getCurrentFault() const {
    return currentFaults;
}



bool FaultSimulation::isFaultActive() const {
    return faultActive;
}



// Setters
void FaultSimulation::setCurrentFault(FaultType fault) {
    currentFaults = {fault};
}



void FaultSimulation::setFaultActive(bool state) {
    faultActive = state;
}



void FaultSimulation::simulateFault(FaultType fault, double severity) {
    // Set current fault and activate it
    std::vector<std::string> effects;
    
    // Set current fault and activate it
    currentFaults.push_back(fault);
    faultActive = true;
    
    // Set current fault and activate it
    // currentFaults updated via push_back above
    
    // Generate effects based on fault type
    switch (fault) {
        case FaultType::SENSOR_DISCONNECT:
            effects.push_back("Sensor disconnected");
            effects.push_back("Limited data available for control systems");
            effects.push_back("Safe Mode activated");
            break;
            
        case FaultType::ENGINE_OVERHEAT:
            effects.push_back("Engine temperature critical");
            effects.push_back("Power output reduced by 50%");
            effects.push_back("Cooling system activated");
            break;
            
        case FaultType::BATTERY_FAULT:
            effects.push_back("Battery fault detected");
            effects.push_back("High voltage system disconnected");
            effects.push_back("Faulty module isolated");
            effects.push_back("Evacuation warning issued");
            break;
            
        case FaultType::BRAKE_FAILURE:
            effects.push_back("Brake system failure detected");
            effects.push_back("Maximum regenerative braking activated");
            effects.push_back("Emergency electromagnetic brake engaged");
            break;
            
        case FaultType::NONE:
        default:
            effects.push_back("No fault active");
            faultActive = false;
            break;
    }


    
    // Effects stored internally
    // No return needed for void function
}



void FaultSimulation::resetFaults() {
    currentFaults.clear();
    faultActive = false;
}

void FaultSimulation::clearFault() {
    currentFaults.clear();
    faultActive = false;
}



std::vector<std::string> FaultSimulation::handleFault(Vehicle& vehicle) {
    if (!faultActive) {
        return {"No fault to handle"};
    }


    
    // Handle fault based on type
    std::vector<std::string> all_actions;
    for (const auto& fault : currentFaults) {
        std::vector<std::string> actions;
        switch (fault) {
            case FaultType::SENSOR_DISCONNECT:
                actions = handleSensorDisconnect(vehicle);
                break;
            case FaultType::ENGINE_OVERHEAT:
                actions = handleEngineOverheat(vehicle);
                break;
            case FaultType::BATTERY_FAULT:
                actions = handleBatteryFault(vehicle);
                break;
            case FaultType::BRAKE_FAILURE:
                actions = handleBrakeFailure(vehicle);
                break;
            case FaultType::NONE:
            default:
                continue;
        }
        all_actions.insert(all_actions.end(), actions.begin(), actions.end());
    }
    return all_actions.empty() ? std::vector<std::string>{"No fault to handle"} : all_actions;


}



std::vector<std::string> FaultSimulation::handleSensorDisconnect(Vehicle& vehicle) {
    (void)vehicle;
    std::vector<std::string> actions;
    
    // As per Database.md, activate Safe Mode
    actions.push_back("Safe Mode activated");
    actions.push_back("Speed limited to 50 km/h");
    actions.push_back("Warning displayed on dashboard");
    
    // In a real implementation, this would call vehicle methods
    // vehicle.getSafetySystem().enterSafeMode();
    // vehicle.setMaxSpeed(50.0);
    // vehicle.getDisplay().addMessage("Sensor disconnected - Safe Mode active", MessageType::WARNING);
    
    return actions;
}



std::vector<std::string> FaultSimulation::handleEngineOverheat(Vehicle& vehicle) {
    (void)vehicle;
    std::vector<std::string> actions;
    
    // As per Database.md, reduce power and activate cooling
    actions.push_back("Power output reduced by 50%");
    actions.push_back("Liquid cooling system activated");
    actions.push_back("Warning displayed on dashboard");
    
    // In a real implementation, this would call vehicle methods
    // vehicle.getEngine().handleOverheat();
    // vehicle.getDisplay().addMessage("Engine overheating - Power reduced", MessageType::WARNING);
    
    return actions;
}



std::vector<std::string> FaultSimulation::handleBatteryFault(Vehicle& vehicle) {
    (void)vehicle;
    std::vector<std::string> actions;
    
    // As per Database.md, disconnect high voltage and isolate faulty module
    actions.push_back("High voltage system disconnected");
    actions.push_back("Faulty battery module isolated");
    actions.push_back("Evacuation warning issued");
    actions.push_back("Emergency services notified");
    
    // In a real implementation, this would call vehicle methods
    // vehicle.getBattery().disconnectHighVoltage();
    // vehicle.getBattery().isolateFaultyModule();
    // vehicle.getDisplay().addMessage("CRITICAL: Battery fault - Evacuate vehicle", MessageType::CRITICAL);
    
    return actions;
}



std::vector<std::string> FaultSimulation::handleBrakeFailure(Vehicle& vehicle) {
    (void)vehicle;
    std::vector<std::string> actions;
    
    // As per Database.md, activate maximum regenerative braking and emergency brake
    actions.push_back("Maximum regenerative braking activated");
    actions.push_back("Electromagnetic emergency brake engaged");
    actions.push_back("Critical warning displayed on dashboard");
    
    // In a real implementation, this would call vehicle methods
    // vehicle.getSafetySystem().activateEmergencyBrake();
    // vehicle.getDisplay().addMessage("CRITICAL: Brake failure - Emergency brake engaged", MessageType::CRITICAL);
    
    return actions;
}



std::string FaultSimulation::getFaultString(FaultType fault) {
    switch (fault) {
        case FaultType::SENSOR_DISCONNECT:
            return "Sensor Disconnect";
        case FaultType::ENGINE_OVERHEAT:
            return "Engine Overheat";
        case FaultType::BATTERY_FAULT:
            return "Battery Fault";
        case FaultType::BRAKE_FAILURE:
            return "Brake Failure";
        case FaultType::NONE:
        default:
            return "None";
    }


}



std::string FaultSimulation::getCurrentFaultString() const {
    std::string result;
    for (const auto& fault : currentFaults) {
        if (!result.empty()) result += ", ";
        result += getFaultString(fault);
    }
    return result.empty() ? "No active faults" : result;
}



std::string FaultSimulation::getStatusString() const {
    std::stringstream ss;
    ss << "Fault Simulation Status:" << std::endl;
    ss << "  Current Fault: " << getCurrentFaultString() << std::endl;
    ss << "  Fault Active: " << (faultActive ? "Yes" : "No") << std::endl;
    
    return ss.str();
}
