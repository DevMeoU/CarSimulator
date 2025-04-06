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
#include "WarningMessages.h"

// Initialize static members
double Vehicle::speed = 0.0;
double Vehicle::distanceTraveled = 0.0;
double Vehicle::brakePressTime = 0.0;
bool Vehicle::doorLocked = true;
bool Vehicle::seatbeltOn = false;
bool Vehicle::engineRunning = false;
bool Vehicle::brakeActive = false;
bool Vehicle::acceleratorActive = false;
bool Vehicle::leftSignalOn = false;
bool Vehicle::rightSignalOn = false;

Vehicle::Vehicle() :
    battery(75.0, 400.0, 450.0),  // Initialize battery with 75 kWh capacity, 400V, 450km range
    engine(150.0, 320.0),  // Initialize engine with 150 kW power and 320 Nm torque
    drivingMode(),  // Uses default constructor which starts in NORMAL mode
    safetySystem(),  // Initialize safety system with default settings
    vehicleData(std::make_shared<VehicleData>()),
    environment(),  // Initialize environmental conditions
    faultSim(),  // Initialize fault simulation system
    sensor(SensorType::SPEED, 1.0),  // Initialize speed sensor with 1.0 update rate
    display(),  // Initialize display system
    gear("P")  // Default gear is Park
    {
        vehicleData->speed = 0.0;
        vehicleData->distance_traveled = 0.0;
        vehicleData->brake = false;
        vehicleData->door_lock = true;
        vehicleData->seat_belt = false;
        vehicleData->running = false;
        vehicleData->gas = false;
        vehicleData->signal_left = false;
        vehicleData->signal_right = false;
        vehicleData->abs_active = false;
        vehicleData->air_condition = 22.0;  // Nhiệt độ điều hòa mặc định 22°C
        vehicleData->altitude = 0.0;        // Độ cao mặc định là mực nước biển
        vehicleData->battery = 100.0;       // Pin đầy khi khởi động
        vehicleData->battery_temp = 25.0;   // Nhiệt độ pin ở mức an toàn
        vehicleData->brake_pressure = 0.0;  // Áp suất phanh ban đầu
        vehicleData->engine_power = 0.0;    // Công suất động cơ ban đầu
        vehicleData->engine_temp = 25.0;    // Nhiệt độ động cơ ở mức an toàn
        vehicleData->engine_torque = 0.0;   // Mô-men xoắn ban đầu
        vehicleData->esp_active = false;    // ESP chưa kích hoạt
        vehicleData->estimated_distance = 0.0; // Quãng đường ước tính
        vehicleData->gear = "P";            // Số P khi khởi động
        vehicleData->mode = drivingMode.getCurrentModeString();  // Đồng bộ mode với DrivingMode
        vehicleData->park = true;           // Đang đỗ xe
        vehicleData->plug_in = false;       // Chưa cắm sạc
        vehicleData->temperature = 25.0;    // Nhiệt độ môi trường
        vehicleData->warning = "";          // Không có cảnh báo
        vehicleData->weather = "CLEAR";      // Thời tiết quang đãng
        vehicleData->wind = 0.0;           // Không có gió
        vehicleData->fan = 0.0;            // Quạt gió tắt
    }

Vehicle::~Vehicle() {}

void Vehicle::setAirConditioningLevel(double level) {
    if (level != 0.0 && (level < 16.0 || level > 32.0)) {
        throw std::invalid_argument("Air conditioning level must be 0.0 (OFF) or between 16.0 and 32.0 (ON)");
    }
    air_conditioning_level = level;
}

double Vehicle::getAirConditioningLevel() const {
    return air_conditioning_level;
}

double Vehicle::getNormalizedBattery() const {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    return std::round(vehicleData->battery) / 100.0;
}

void Vehicle::update(double deltaTime) {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);

    // Initialize natural deceleration rate
    double naturalDeceleration = 0.0;

    // Update vehicle speed based on acceleration/brake state
    if (vehicleData->gas && !vehicleData->brake) {
        // Apply throttle to speed, accumulating over time
        double acceleration = engine.getThrottle() * 3.6; // Convert to km/h
        vehicleData->speed += acceleration * deltaTime;
        // Update member variables
        speed = vehicleData->speed;
        acceleratorActive = true;
        brakeActive = false;
    } else if (vehicleData->brake && !vehicleData->gas) {
        double deceleration = safetySystem.getBrakePower() * 3.6; // Convert to km/h
        vehicleData->speed -= deceleration * deltaTime;
        vehicleData->speed = std::max(0.0, vehicleData->speed); // Ensure speed doesn't go negative
        // Update member variables
        speed = vehicleData->speed;
        acceleratorActive = false;
        brakeActive = true;
    } else if (!vehicleData->gas && !vehicleData->brake) {
        // Natural deceleration when neither gas nor brake is pressed
        naturalDeceleration = vehicleData->speed * 0.02; // 2% speed loss per update
        vehicleData->speed -= naturalDeceleration;
        if (vehicleData->speed < 2.5) vehicleData->speed = 0; // Stop completely when very slow
        // Update member variables
        speed = vehicleData->speed;
        acceleratorActive = false;
        brakeActive = false;
    }

    // Giới hạn tốc độ cho số lùi
    if (vehicleData->gear == "R" && vehicleData->speed > 25.0) {
        vehicleData->speed = 25.0;
        speed = vehicleData->speed;
        vehicleData->speed -= naturalDeceleration;
        if (vehicleData->speed < 2.5) vehicleData->speed = 0; // Stop completely when very slow
        // Update member variables
        speed = vehicleData->speed;
        acceleratorActive = false;
        brakeActive = false;
    } else if (vehicleData->gas && vehicleData->brake) {
        // If both gas and brake are pressed, prioritize brake for safety
        double deceleration = safetySystem.getBrakePower() * 3.6; // Convert to km/h
        vehicleData->speed -= deceleration * deltaTime;
        vehicleData->speed = std::max(0.0, vehicleData->speed); // Ensure speed doesn't go negative
        // Update member variables
        speed = vehicleData->speed;
        acceleratorActive = false;
        brakeActive = true;
    }
    
    // Apply speed limits
    vehicleData->speed = std::round(std::max(0.0, std::min(vehicleData->speed, drivingMode.getMaxSpeedLimit())) * 10) / 10;
    
    // Update distance traveled
    vehicleData->distance_traveled = std::round((vehicleData->distance_traveled + vehicleData->speed * deltaTime / 3600) * 100) / 100;
    
    // Update battery state
    battery.updateCharge(deltaTime, vehicleData->speed);
    vehicleData->battery = std::round(battery.getChargePercentage());
    
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
    if (vehicleData->battery_temp > 50) {
        vehicleData->warning = WARNING_BATTERY_TEMP_HIGH;
    } else if (vehicleData->engine_temp > 120) {
        vehicleData->warning = WARNING_HIGH_ENGINE_TEMP;
    } else if (!vehicleData->seat_belt && vehicleData->speed > 0) {
        vehicleData->warning = WARNING_SEATBELT_HIGH_SPEED;
    } else if (!vehicleData->door_lock && vehicleData->speed > 20) {
        vehicleData->warning = WARNING_SYSTEM_ERROR;
    } else if (vehicleData->battery < 20 && !vehicleData->plug_in) {
        vehicleData->warning = WARNING_LOW_BATTERY;
    } else {
        vehicleData->warning = WARNING_NONE;
    }
    
    // Update display with additional warnings
    display.showStatus(vehicleData->speed, battery.getChargePercentage(), vehicleData->distance_traveled);
    
    // Update auxiliary parameters
    vehicleData->brake_pressure = std::round(safetySystem.getBrakePower() * 100) / 100;
    vehicleData->safetyStatus = safetySystem.getStatusString();
    vehicleData->environmentTemp = std::round(ambient_temp * 10) / 10;
    vehicleData->isSafe = safetySystem.checkStartConditions(*vehicleData);
    
    // Update mode and gear status
    vehicleData->mode = drivingMode.getCurrentModeString();
    // Ensure gear status is synchronized between static member and vehicleData
    vehicleData->gear = gear;
    
    // Update environmental conditions
    vehicleData->wind = environment.getWindSpeed();
    vehicleData->weather = environment.getWeatherString();
    vehicleData->altitude = environment.getAltitude();
    
    // Synchronize air_condition and ac_state value from environment to vehicleData
    vehicleData->ac_state = getAirConditioningLevel() > 0;
    double currentAC = getAirConditioningLevel();
    vehicleData->air_condition = currentAC;
    setAirConditioningLevel(currentAC); // Ensure consistency
    
    // Không cập nhật mức quạt gió tự động nữa vì đã được điều khiển bởi người dùng
    
    // Update safety system status
    vehicleData->abs_active = safetySystem.isAbsActive();
    vehicleData->esp_active = safetySystem.isEspActive();
    
    // Update timestamp
    vehicleData->timestamp_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
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

void Vehicle::brake(double intensity) {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    
    // Khi intensity > 0 là đang phanh, intensity = 0 là nhả phanh
    vehicleData->brake = (intensity > 0);
    safetySystem.applyBrake(intensity);
}

void Vehicle::accelerate(double intensity) {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    
    // Kiểm tra hộp số trước khi tăng tốc
    if (gear == "P" || gear == "N") {
        vehicleData->warning = WARNING_INVALID_GEAR_ACCELERATION;
        return;
    }
    
    acceleratorActive = true;
    vehicleData->gas = true;
    // Call the engine's setThrottle method with the given intensity
    engine.setThrottle(intensity);
}

void Vehicle::decelerate(double intensity) {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);

    acceleratorActive = false;
    vehicleData->gas = false;
    engine.setThrottle(-intensity);
}

bool Vehicle::changeDrivingMode(DrivingModeType mode) {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    double& currentSpeed = vehicleData->speed;
    bool success = drivingMode.changeMode(mode, currentSpeed);
    if (success) {
        vehicleData->mode = DrivingMode::getModeString(mode);
        std::cout << "[Vehicle] Successfully changed mode to: " << vehicleData->mode << std::endl;
    }
    return success;
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
    return std::round(vehicleData->speed * 10) / 10;
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

double Vehicle::getFan() const {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    return vehicleData->fan;
}

void Vehicle::setFan(double level) {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    // Giới hạn mức quạt từ 0-5
    vehicleData->fan = std::max(0.0, std::min(5.0, level));
}

std::string Vehicle::getGear() const {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    return vehicleData->gear;
}

void Vehicle::setGear(const std::string& gear) {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    
    // Kiểm tra điều kiện tốc độ trước khi cho phép chuyển số
    if (vehicleData->speed > 0 && (gear == "P" || gear == "N")) {
        vehicleData->warning = WARNING_INVALID_GEAR_CHANGE;
        return;
    }
    
    if (gear == "P" || gear == "R" || gear == "N" || gear == "D") {
        this->gear = gear;
        vehicleData->gear = gear;
        
        // Additional logic for gear changes
        if (gear == "P") {
            vehicleData->park = true;
            vehicleData->speed = 0;
        } else if (gear == "R") {
            vehicleData->park = false;
            // Giới hạn tốc độ tối đa cho số lùi là 25 km/h
            if (vehicleData->speed > 25.0) {
                vehicleData->speed = 25.0;
            }
        } else {
            vehicleData->park = false;
        }
        
        // Update display with new gear information
        display.showGear(gear);
        
        // Log gear change for debugging
        std::cout << "[Vehicle] Gear changed to: " << gear << std::endl;
        std::cout << "[Vehicle] Current gear in vehicleData: " << vehicleData->gear << std::endl;
    } else {
        std::cerr << "[Vehicle] Invalid gear: " << gear << std::endl;
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

std::string Vehicle::getWarningMessages() const {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    
    // Return appropriate warning based on vehicle state
    if (!seatbeltOn && speed > 20) {
        return WARNING_SEATBELT_HIGH_SPEED;
    } else if (speed > 100 && safetySystem.getBrakePower() < 50) {
        return WARNING_LOW_BRAKE_PRESSURE;
    } else if (speed > 0 && vehicleData->park) {
        return WARNING_PARKING_WHILE_MOVING;
    } else if (speed > 10 && vehicleData->gear == "R") {
        return WARNING_REVERSE_HIGH_SPEED;
    } else if (battery.getChargePercentage() < 20) {
        return WARNING_LOW_BATTERY;
    } else if (engine.getTemperature() > 90) {
        return WARNING_HIGH_ENGINE_TEMP;
    } else if (battery.getTemperature() > 50) {
        return WARNING_BATTERY_TEMP_HIGH;
    // } else if (safetySystem.getOilPressure() < 30) {
    //     return WARNING_LOW_OIL_PRESSURE;
    } else if (environment.getAltitude() > 3000) {
        return WARNING_HIGH_ALTITUDE;
    } else if (environment.getTemperature() > 40 || environment.getTemperature() < -10) {
        return WARNING_EXTREME_TEMPERATURE;
    } else if (environment.getWeather() == WeatherType::SNOW || environment.getWeather() == WeatherType::FOG) {
        return WARNING_SEVERE_WEATHER;
    } else if (battery.hasChargingError()) {
        return WARNING_CHARGING_ERROR;
    } else if (safetySystem.hasBrakeSystemError()) {
        return WARNING_BRAKE_SYSTEM;
    } else if (safetySystem.hasSystemError()) {
        return WARNING_SYSTEM_ERROR;
    }
    
    return WARNING_NONE;
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
