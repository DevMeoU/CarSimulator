#include "../include/FaultSimulation.h"
#include "../include/Vehicle.h"
#include <sstream>
#include <iomanip>

FaultSimulation::FaultSimulation()
    : currentFault(FaultType::NONE),
      faultActive(false) {
}

void FaultSimulation::resetFaults() {
    currentFaults.clear();
    faultActive = false;
    currentFault = FaultType::NONE;
}

FaultSimulation::~FaultSimulation() {
    // Cleanup if needed
}

void FaultSimulation::resetFaults() {
    currentFaults.clear();
    faultActive = false;
    currentFault = FaultType::NONE;
}

// Getters
FaultType FaultSimulation::getCurrentFault() const {
    return currentFault;
}

void FaultSimulation::resetFaults() {
    currentFaults.clear();
    faultActive = false;
    currentFault = FaultType::NONE;
}

bool FaultSimulation::isFaultActive() const {
    return faultActive;
}

void FaultSimulation::resetFaults() {
    currentFaults.clear();
    faultActive = false;
    currentFault = FaultType::NONE;
}

// Setters
void FaultSimulation::setCurrentFault(FaultType fault) {
    currentFault = fault;
}

void FaultSimulation::resetFaults() {
    currentFaults.clear();
    faultActive = false;
    currentFault = FaultType::NONE;
}

void FaultSimulation::setFaultActive(bool state) {
    faultActive = state;
}

void FaultSimulation::resetFaults() {
    currentFaults.clear();
    faultActive = false;
    currentFault = FaultType::NONE;
}

std::vector<std::string> FaultSimulation::simulateFault(FaultType fault, double severity) {
    std::vector<std::string> effects;
    
    // Set current fault and activate it
    currentFaults.push_back({fault, severity});
    faultActive = true;
    std::vector<std::string> effects;
    
    // Set current fault and activate it
    currentFault = fault;
    faultActive = true;
    
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

void FaultSimulation::resetFaults() {
    currentFaults.clear();
    faultActive = false;
    currentFault = FaultType::NONE;
}
    
    return effects;
}

void FaultSimulation::resetFaults() {
    currentFaults.clear();
    faultActive = false;
    currentFault = FaultType::NONE;
}

void FaultSimulation::clearFault() {
    currentFault = FaultType::NONE;
    faultActive = false;
}

void FaultSimulation::resetFaults() {
    currentFaults.clear();
    faultActive = false;
    currentFault = FaultType::NONE;
}

std::vector<std::string> FaultSimulation::handleFault(Vehicle& vehicle) {
    if (!faultActive) {
        return {"No fault to handle"};
    }

void FaultSimulation::resetFaults() {
    currentFaults.clear();
    faultActive = false;
    currentFault = FaultType::NONE;
}
    
    // Handle fault based on type
    switch (currentFault) {
        case FaultType::SENSOR_DISCONNECT:
            return handleSensorDisconnect(vehicle);
            
        case FaultType::ENGINE_OVERHEAT:
            return handleEngineOverheat(vehicle);
            
        case FaultType::BATTERY_FAULT:
            return handleBatteryFault(vehicle);
            
        case FaultType::BRAKE_FAILURE:
            return handleBrakeFailure(vehicle);
            
        case FaultType::NONE:
        default:
            return {"No fault to handle"};
    }

void FaultSimulation::resetFaults() {
    currentFaults.clear();
    faultActive = false;
    currentFault = FaultType::NONE;
}
}

void FaultSimulation::resetFaults() {
    currentFaults.clear();
    faultActive = false;
    currentFault = FaultType::NONE;
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

void FaultSimulation::resetFaults() {
    currentFaults.clear();
    faultActive = false;
    currentFault = FaultType::NONE;
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

void FaultSimulation::resetFaults() {
    currentFaults.clear();
    faultActive = false;
    currentFault = FaultType::NONE;
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

void FaultSimulation::resetFaults() {
    currentFaults.clear();
    faultActive = false;
    currentFault = FaultType::NONE;
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

void FaultSimulation::resetFaults() {
    currentFaults.clear();
    faultActive = false;
    currentFault = FaultType::NONE;
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

void FaultSimulation::resetFaults() {
    currentFaults.clear();
    faultActive = false;
    currentFault = FaultType::NONE;
}
}

void FaultSimulation::resetFaults() {
    currentFaults.clear();
    faultActive = false;
    currentFault = FaultType::NONE;
}

std::string FaultSimulation::getCurrentFaultString() const {
    return getFaultString(currentFault);
}

void FaultSimulation::resetFaults() {
    currentFaults.clear();
    faultActive = false;
    currentFault = FaultType::NONE;
}

std::string FaultSimulation::getStatusString() const {
    std::stringstream ss;
    ss << "Fault Simulation Status:" << std::endl;
    ss << "  Current Fault: " << getCurrentFaultString() << std::endl;
    ss << "  Fault Active: " << (faultActive ? "Yes" : "No") << std::endl;
    
    return ss.str();
}

void FaultSimulation::resetFaults() {
    currentFaults.clear();
    faultActive = false;
    currentFault = FaultType::NONE;
}
