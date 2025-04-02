#include "Vehicle.h"
#include "Battery.h"
#include "Engine.h"
#include "DrivingMode.h"
#include "SafetySystem.h"
#include "EnvironmentalCondition.h"
#include "FaultSimulation.h"
#include "Sensor.h"
#include "Display.h"
#include <string>
#include <atomic>
#include <mutex>

Vehicle::Vehicle() :
    vehicleData(std::make_shared<VehicleData>()),
    sensor(SensorType::SPEED, 1.0),
    speed(0),
    distanceTraveled(0),
    brakePressTime(0),
    doorLocked(true),
    seatbeltOn(false), 
    engineRunning(false), 
    brakeActive(false),
    acceleratorActive(false), 
    leftSignalOn(false), 
    rightSignalOn(false) {}

Vehicle::~Vehicle() {}

void Vehicle::update(double deltaTime) {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    
    // Update vehicle speed based on acceleration/brake state
    if (acceleratorActive && !brakeActive) {
        speed += engine.getThrottle() * deltaTime;
    } else if (brakeActive && !acceleratorActive) {
        speed -= safetySystem.getBrakePower() * deltaTime;
    }
    
    // Apply speed limits
    speed = std::max(0.0, std::min(speed, drivingMode.getMaxSpeedLimit()));
    
    // Update distance traveled
    distanceTraveled += speed * deltaTime / 3600; // Convert from km/h to km/s
    
    // Update battery state
    battery.updateCharge(deltaTime, speed);
    
    // Update sensor readings
    sensor.update(speed);
    
    // Update display
    display.showStatus(speed, battery.getChargePercentage(), distanceTraveled);
}

bool Vehicle::startEngine() {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    
    // Check safety conditions
    if (!safetySystem.checkStartConditions(*vehicleData)) {
        return false;
    }
    
    engineRunning = true;
    engine.start();
    return true;
}

void Vehicle::stopEngine() {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    
    engineRunning = false;
    engine.stop();
    speed = 0;
}

void Vehicle::pressBrake(double seconds) {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    brakePressTime = seconds;
    brakeActive = true;
}

void Vehicle::brake(double intensity) {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    
    brakeActive = true;
    safetySystem.applyBrake(intensity);
}

void Vehicle::accelerate(double intensity) {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    
    acceleratorActive = true;
    engine.setThrottle(intensity);
}

bool Vehicle::changeDrivingMode(DrivingModeType mode) {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    return drivingMode.changeMode(mode, getSpeed());
}

void Vehicle::updateEnvironment(double temperature, double humidity, double windSpeed, double altitude) {
    environment.setTemperature(temperature);
    environment.setHumidity(humidity);
    environment.setWindSpeed(windSpeed);
    environment.setAltitude(altitude);
}

double Vehicle::getCurrentMaxSpeed() const {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    return drivingMode.getMaxSpeedLimit();
}

double Vehicle::getSpeed() const {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    return speed;
}

double Vehicle::getDistanceTraveled() const {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    return distanceTraveled;
}

double Vehicle::getBatteryLoad() const {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    return battery.getCurrentLoad();
}

double Vehicle::getBrakePressTime() const {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    return brakePressTime;
}

bool Vehicle::isDoorLocked() const {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    return doorLocked;
}

void Vehicle::setDoorLocked(bool locked) {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    doorLocked = locked;
}

bool Vehicle::isSeatbeltOn() const {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    return seatbeltOn;
}

void Vehicle::setSeatbeltOn(bool on) {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    seatbeltOn = on;
}

bool Vehicle::isEngineRunning() const {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    return engineRunning;
}

bool Vehicle::isBrakeActive() const {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    return brakeActive;
}

bool Vehicle::isAcceleratorActive() const {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    return acceleratorActive;
}

bool Vehicle::isLeftSignalOn() const {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    return leftSignalOn;
}

void Vehicle::setLeftSignalOn(bool on) {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    leftSignalOn = on;
}

bool Vehicle::isRightSignalOn() const {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    return rightSignalOn;
}

void Vehicle::setRightSignalOn(bool on) {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    rightSignalOn = on;
}

void Vehicle::setGear(const std::string& gear) {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    if (gear == "P" || gear == "R" || gear == "N" || gear == "D") {
        vehicleData->gear = gear;
        
        // Additional logic for gear changes
        if (gear == "P") {
            vehicleData->park = true;
            vehicleData->speed = 0;
        } else {
            vehicleData->park = false;
        }
    }
}

void Vehicle::setParking(bool on) {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    
    // Safety checks
    if (on && vehicleData->speed > 0) {
        vehicleData->warning = "Emergency parking brake activated!";
        vehicleData->speed = 0;
    }
    
    vehicleData->park = on;
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
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    return "Speed: " + std::to_string(speed) + " km/h\n" +
           "Distance: " + std::to_string(distanceTraveled) + " km\n" +
           "Battery: " + std::to_string(battery.getChargeLevel()) + "%\n" +
           "Engine: " + (engineRunning ? "Running" : "Stopped") + "\n" +
           "Doors: " + (doorLocked ? "Locked" : "Unlocked") + "\n" +
           "Seatbelt: " + (seatbeltOn ? "On" : "Off");
}

void Vehicle::simulateFault(FaultType faultType, double severity) {
    faultSim.simulateFault(faultType, severity);
}

void Vehicle::resetFaults() {
    faultSim.resetFaults();
}

std::string Vehicle::getWeatherCondition() const {
    switch(environment.getWeather()) {
        case WeatherType::CLEAR: return "Clear";
        case WeatherType::RAIN: return "Rain";
        case WeatherType::SNOW: return "Snow";
        case WeatherType::FOG: return "Fog";
        default: return "Unknown";
    }
}

void Vehicle::emergencyStop() {
    safetySystem.activateEmergencyBrake();
    speed = 0;
    acceleratorActive = false;
    brakeActive = true;
}

bool Vehicle::isEmergencyStopped() const {
    return brakeActive && !engineRunning && speed == 0;
}
