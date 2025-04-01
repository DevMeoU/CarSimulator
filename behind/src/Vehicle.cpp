#include "../include/Vehicle.h"
#include <iostream>
#include <sstream>
#include <iomanip>

Vehicle::Vehicle()
    : sensor(SensorType::SPEED),
      speed(0.0), distanceTraveled(0.0), brakePressTime(0.0),
      doorLocked(false), seatbeltOn(false), engineRunning(false),
      brakeActive(false), acceleratorActive(false),
      leftSignalOn(false), rightSignalOn(false) {
}

Vehicle::~Vehicle() {
}

void Vehicle::update(double deltaTime) {
    // Update vehicle systems
    double throttlePosition = acceleratorActive ? 1.0 : 0.0;
    engine.updatePower(throttlePosition, drivingMode.getCurrentMode());
    battery.discharge(engine.getCurrentPower() * deltaTime / battery.getCapacity());
    
    // Update speed based on acceleration/braking
    if (brakeActive) {
        // Apply braking
        double deceleration = 10.0; // m/s^2
        speed = std::max(0.0, speed - (deceleration * 3.6 * deltaTime)); // Convert m/s^2 to km/h/s
        
        // Regenerative braking - recover some energy
        if (speed > 5.0) { // Only apply regenerative braking above 5 km/h
            double recoveryFactor = 0.2; // 20% energy recovery
            double recoveredEnergy = engine.getCurrentPower() * deltaTime * recoveryFactor;
            battery.charge(recoveredEnergy / battery.getCapacity());
        }
        
        brakeActive = false; // Reset brake flag after applying
    }
    
    if (acceleratorActive && engineRunning) {
        // Apply acceleration
        double acceleration = 5.0; // m/s^2
        double maxSpeed = getCurrentMaxSpeed();
        speed = std::min(maxSpeed, speed + (acceleration * 3.6 * deltaTime)); // Convert m/s^2 to km/h/s
        
        acceleratorActive = false; // Reset accelerator flag after applying
    }
    
    // Update distance traveled
    distanceTraveled += (speed / 3600.0) * deltaTime; // km/h * h = km
    
    // Check safety systems
    if (speed > 0 && !seatbeltOn) {
        display.addMessage("Please fasten seatbelt", MessageType::WARNING);
    }
    
    if (speed > 120.0) {
        display.addMessage("High speed warning", MessageType::WARNING);
    }
    
    // Update sensors with current state
    sensor.update(speed);
    
    // Update engine and battery temperature
    engine.updateTemperature(environment.getTemperature(), throttlePosition);
    battery.updateTemperature(environment.getTemperature(), throttlePosition);
    
    // Check for faults
    if (faultSim.isFaultActive()) {
        auto actions = faultSim.handleFault(*this);
        for (const auto& action : actions) {
            display.addMessage(action, MessageType::CRITICAL);
        }
    }
}

bool Vehicle::startEngine() {
    if (!doorLocked) {
        display.addMessage("Cannot start engine: Doors not locked", MessageType::ERROR_MSG);
        return false;
    }
    
    if (brakePressTime < 3.0) {
        display.addMessage("Cannot start engine: Press brake for 3 seconds", MessageType::ERROR_MSG);
        return false;
    }
    
    engineRunning = true;
    display.addMessage("Engine started", MessageType::INFO);
    return true;
}

void Vehicle::stopEngine() {
    engineRunning = false;
    speed = 0.0;
    display.addMessage("Engine stopped", MessageType::INFO);
}

void Vehicle::pressBrake(double seconds) {
    brakePressTime = seconds;
    brake(1.0); // Apply full brake
}

void Vehicle::brake(double intensity) {
    if (intensity < 0.0 || intensity > 1.0) {
        std::cerr << "Brake intensity must be between 0 and 1" << std::endl;
        return;
    }
    
    brakeActive = true;
}

void Vehicle::accelerate(double intensity) {
    if (intensity < 0.0 || intensity > 1.0) {
        std::cerr << "Acceleration intensity must be between 0 and 1" << std::endl;
        return;
    }
    
    if (!engineRunning) {
        display.addMessage("Cannot accelerate: Engine not running", MessageType::ERROR_MSG);
        return;
    }
    
    acceleratorActive = true;
}

bool Vehicle::changeDrivingMode(DrivingModeType mode) {
    // Check if current speed is within the limit of the new mode
    double newModeLimit = drivingMode.getMaxSpeedLimit(mode);
    
    if (speed > newModeLimit) {
        display.addMessage("Reduce speed before changing mode", MessageType::WARNING);
        return false;
    }
    
    drivingMode.setCurrentMode(mode);
    display.addMessage("Driving mode changed to " + drivingMode.getCurrentModeString(), MessageType::INFO);
    return true;
}

double Vehicle::getCurrentMaxSpeed() const {
    return drivingMode.getMaxSpeedLimit();
}

double Vehicle::getSpeed() const {
    return speed;
}

double Vehicle::getDistanceTraveled() const {
    return distanceTraveled;
}

double Vehicle::getBrakePressTime() const {
    return brakePressTime;
}

bool Vehicle::isDoorLocked() const {
    return doorLocked;
}

void Vehicle::setDoorLocked(bool locked) {
    doorLocked = locked;
    display.addMessage(locked ? "Doors locked" : "Doors unlocked", MessageType::INFO);
}

bool Vehicle::isSeatbeltOn() const {
    return seatbeltOn;
}

void Vehicle::setSeatbeltOn(bool on) {
    seatbeltOn = on;
    display.addMessage(on ? "Seatbelt fastened" : "Seatbelt unfastened", MessageType::INFO);
}

bool Vehicle::isEngineRunning() const {
    return engineRunning;
}

bool Vehicle::isBrakeActive() const {
    return brakeActive;
}

bool Vehicle::isAcceleratorActive() const {
    return acceleratorActive;
}

bool Vehicle::isLeftSignalOn() const {
    return leftSignalOn;
}

void Vehicle::setLeftSignalOn(bool on) {
    leftSignalOn = on;
    if (on) {
        rightSignalOn = false; // Turn off right signal when left is turned on
    }
}

bool Vehicle::isRightSignalOn() const {
    return rightSignalOn;
}

void Vehicle::setRightSignalOn(bool on) {
    rightSignalOn = on;
    if (on) {
        leftSignalOn = false; // Turn off left signal when right is turned on
    }
}

Battery& Vehicle::getBattery() {
    return battery;
}

Engine& Vehicle::getEngine() {
    return engine;
}

DrivingMode& Vehicle::getDrivingMode() {
    return drivingMode;
}

SafetySystem& Vehicle::getSafetySystem() {
    return safetySystem;
}

EnvironmentalCondition& Vehicle::getEnvironment() {
    return environment;
}

FaultSimulation& Vehicle::getFaultSim() {
    return faultSim;
}

Sensor& Vehicle::getSensor() {
    return sensor;
}

Display& Vehicle::getDisplay() {
    return display;
}

std::string Vehicle::getStatusString() const {
    std::stringstream ss;
    ss << "Vehicle Status:" << std::endl;
    ss << "  Speed: " << std::fixed << std::setprecision(1) << speed << " km/h" << std::endl;
    ss << "  Distance Traveled: " << std::fixed << std::setprecision(2) << distanceTraveled << " km" << std::endl;
    ss << "  Engine: " << (engineRunning ? "Running" : "Stopped") << std::endl;
    ss << "  Driving Mode: " << drivingMode.getCurrentModeString() << std::endl;
    ss << "  Doors: " << (doorLocked ? "Locked" : "Unlocked") << std::endl;
    ss << "  Seatbelt: " << (seatbeltOn ? "Fastened" : "Unfastened") << std::endl;
    ss << "  Turn Signals: ";
    if (leftSignalOn) ss << "Left";
    else if (rightSignalOn) ss << "Right";
    else ss << "Off";
    ss << std::endl;
    
    return ss.str();
}
