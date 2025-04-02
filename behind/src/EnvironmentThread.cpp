#include "EnvironmentThread.h"
#include "VehicleData.h"
#include <thread>
#include <chrono>
#include <cmath>

EnvironmentThread::EnvironmentThread(std::shared_ptr<VehicleData> data) 
    : vehicleData(data), running(false) {}

void EnvironmentThread::start() {
    running = true;
    thread = std::thread(&EnvironmentThread::threadFunction, this);
}

void EnvironmentThread::stop() {
    running = false;
    if (thread.joinable()) {
        thread.join();
    }
}

void EnvironmentThread::threadFunction() {
    while (running) {
        updateEnvironmentalConditions();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void EnvironmentThread::updateEnvironmentalConditions() {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    
    // Cập nhật nhiệt độ môi trường (-10°C đến 40°C)
    vehicleData->temperature = 15.0 + 25.0 * sin(std::chrono::system_clock::now().time_since_epoch().count() / 1000000000.0);
    
    // Cập nhật độ cao (0m đến 3000m)
    vehicleData->altitude = 500.0 + 2500.0 * sin(std::chrono::system_clock::now().time_since_epoch().count() / 2000000000.0);
    
    // Cập nhật thời tiết dựa trên nhiệt độ
    if (vehicleData->temperature < 0) {
        vehicleData->weather = "Snow";
    } else if (vehicleData->temperature > 30) {
        vehicleData->weather = "Sunny";
    } else {
        vehicleData->weather = "Rain";
    }
    
    // Cập nhật gió (0-50 km/h)
    vehicleData->wind = 25.0 + 25.0 * sin(std::chrono::system_clock::now().time_since_epoch().count() / 1500000000.0);
    
    // Cập nhật nhiệt độ điều hòa (18-26°C)
    vehicleData->air_condition = 22.0 + 4.0 * sin(std::chrono::system_clock::now().time_since_epoch().count() / 1800000000.0);
}