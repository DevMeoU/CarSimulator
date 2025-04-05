#include <iostream>
#include <string>
#include <atomic>
#include <mutex>
#include <cmath>
#include "Vehicle.h"
#include "Battery.h"
#include "Engine.h"
#include "DrivingMode.h"
#include "SafetySystem.h"
#include "EnvironmentalCondition.h"
#include "FaultSimulation.h"
#include "Sensor.h"
#include "Display.h"

Vehicle::Vehicle() :
    vehicleData(std::make_shared<VehicleData>()),
    sensor(SensorType::SPEED, 1.0) {
        vehicleData->speed = 0.0;
        vehicleData->distance_traveled = 0.0;
        vehicleData->brake = false;
        vehicleData->door_lock = true;
        vehicleData->seat_belt = false;
        vehicleData->running = false;
        vehicleData->brake = false;
        vehicleData->gas = false;
        vehicleData->signal_left = false;
        vehicleData->signal_right = false;
        vehicleData->abs_active = false;
        vehicleData->air_condition = 0;
        vehicleData->altitude = 0;
        vehicleData->battery = 0.0;
        vehicleData->battery_temp = 0;
        vehicleData->brake_pressure = 0;
        vehicleData->engine_power = 0;
        vehicleData->engine_temp = 0;
        vehicleData->engine_torque = 0;
        vehicleData->esp_active = false;
        vehicleData->estimated_distance = 0;
        vehicleData->gear = "";
        vehicleData->mode = "";
        vehicleData->park = false;
        vehicleData->plug_in = false;
        vehicleData->temperature = 0;
        vehicleData->warning = "";
        vehicleData->weather = "";
        vehicleData->wind = 0;
    }

Vehicle::~Vehicle() {}

void Vehicle::update(double deltaTime) {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    
    // Update vehicle speed based on acceleration/brake state
    if (vehicleData->gas && !vehicleData->brake) {
        vehicleData->speed += engine.getThrottle() * deltaTime;
        vehicleData->speed = std::max(0.0, vehicleData->speed); // Ensure speed doesn't go negative
    } else if (vehicleData->brake && !vehicleData->gas) {
        vehicleData->speed -= safetySystem.getBrakePower() * deltaTime;
        vehicleData->speed = std::max(0.0, vehicleData->speed); // Ensure speed doesn't go negative
    } else if (!vehicleData->gas && !vehicleData->brake) {
        // Natural deceleration when neither gas nor brake is pressed
        vehicleData->speed *= 0.98; // Gradual deceleration
        if (vehicleData->speed < 0.1) vehicleData->speed = 0; // Stop completely when very slow
    }
    
    // Apply speed limits
    vehicleData->speed = std::round(std::max(0.0, std::min(vehicleData->speed, drivingMode.getMaxSpeedLimit())) * 10) / 10;
    
    // Update distance traveled
    vehicleData->distance_traveled = std::round((vehicleData->distance_traveled + vehicleData->speed * deltaTime / 3600) * 100) / 100;
    
    // Update battery state
    battery.updateCharge(deltaTime, vehicleData->speed);
    vehicleData->battery = std::round(battery.getChargePercentage() * 100) / 100;
    
    // Calculate estimated distance based on battery and speed (electric vehicle formula)
    double energy_efficiency = 8 - 0.05 * vehicleData->speed;
    vehicleData->estimated_distance = std::round((vehicleData->battery * energy_efficiency / 100) * 100) / 100;
    
    // Calculate engine temperature with safety checks
    double ambient_temp = environment.getTemperature();
    vehicleData->engine_temp = std::round((ambient_temp + vehicleData->engine_power / 10) * 10) / 10;
    
    // Reduce engine power if overheating (from depend.md)
    if (vehicleData->engine_temp > 100) {
        vehicleData->engine_power *= 0.8;
    }
    
    // Calculate battery drop rate with efficiency factor (from depend.md)
    vehicleData->battery_drop_rate = std::round((vehicleData->engine_power * 0.1 + vehicleData->air_condition * 2) * 100) / 100;
    
    // Battery temperature calculation with efficiency factor (from depend.md)
    vehicleData->battery_temp = std::round((25 + (vehicleData->battery - 50) * 0.2) * 10) / 10;
    
    // Update sensor readings
    sensor.update(vehicleData->speed);
    
    // Generate warnings based on conditions (from depend.md)
    if (vehicleData->battery < 20 && !vehicleData->plug_in) {
        vehicleData->warning = "Cảnh báo: Pin yếu!";
    } else if (vehicleData->battery_temp > 50) {
        vehicleData->warning = "Cảnh báo: Quá nhiệt pin!";
    } else if (vehicleData->engine_temp > 120) {
        vehicleData->warning = "Cảnh báo: Động cơ quá nhiệt!";
    } else if (!vehicleData->seat_belt && vehicleData->speed > 0) {
        vehicleData->warning = "Cảnh báo: Chưa thắt dây an toàn!";
    } else if (!vehicleData->door_lock && vehicleData->speed > 20) {
        vehicleData->warning = "Cảnh báo: Cửa chưa khóa!";
    } else {
        vehicleData->warning = "";
    }
    
    // Update display with additional warnings
    display.showStatus(vehicleData->speed, battery.getChargePercentage(), vehicleData->distance_traveled);
    
    // Update auxiliary parameters
    vehicleData->brake_pressure = std::round(safetySystem.getBrakePower() * 100) / 100;
    vehicleData->safetyStatus = safetySystem.getStatusString();
    vehicleData->environmentTemp = std::round(ambient_temp * 10) / 10;
    vehicleData->isSafe = safetySystem.checkStartConditions(*vehicleData);
}

bool Vehicle::startEngine() {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    
    // Check safety conditions
    if (!safetySystem.checkStartConditions(*vehicleData)) {
        return false;
    }
    
    vehicleData->running = true;
    engine.start();
    return true;
}

void Vehicle::stopEngine() {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    
    vehicleData->running = false;
    engine.stop();
    vehicleData->speed = 0;
}

void Vehicle::pressBrake(double seconds) {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    vehicleData->brake = true;
}

void Vehicle::brake(double intensity) {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    
    vehicleData->brake = true;
    safetySystem.applyBrake(intensity);
}

void Vehicle::accelerate(double intensity) {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    
    acceleratorActive = true;
    engine.setThrottle(intensity);
}

void Vehicle::decelerate(double intensity) {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);

    acceleratorActive = false;
    engine.setThrottle(-intensity);
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
    return std::round(speed * 10) / 10;
}

double Vehicle::getDistanceTraveled() const {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    return std::round(distanceTraveled * 100) / 100;
}

double Vehicle::getBatteryLoad() const {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    return std::round(battery.getCurrentLoad() * 100) / 100;
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
