#include "MainThread.h"
#include <chrono>
#include <cmath>
#include "VehicleData.h"

MainThread::MainThread(std::shared_ptr<VehicleData> data) 
    : vehicleData(data), running(false) {}

void MainThread::start() {
    running = true;
    thread = std::thread(&MainThread::threadFunction, this);
}

void MainThread::stop() {
    running = false;
    if (thread.joinable()) {
        thread.join();
    }
}

void MainThread::threadFunction() {
    while (running) {
        updateVehicleState();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void MainThread::updateVehicleState() {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    
    // Tính toán hiệu suất động cơ dựa trên chế độ lái
    double power_multiplier = 1.0;
    if (vehicleData->mode == "Sport") {
        power_multiplier = 1.3;
    } else if (vehicleData->mode == "Eco") {
        power_multiplier = 0.8;
    } else if (vehicleData->mode == "Snow/Off-road") {
        power_multiplier = 0.7;
    }
    
    // Cập nhật tốc độ dựa trên ga/phanh
    if (vehicleData->gas) {
        vehicleData->speed += 0.5 * power_multiplier;
    } else if (vehicleData->brake) {
        vehicleData->speed -= 1.0;
    }
    
    // Ảnh hưởng của môi trường lên tốc độ
    if (vehicleData->weather == "Snow") {
        vehicleData->speed *= 0.8; // Giảm 20% tốc độ trên tuyết
    } else if (vehicleData->weather == "Rain") {
        vehicleData->speed *= 0.9; // Giảm 10% tốc độ khi trời mưa
    }
    
    // Ảnh hưởng của độ cao
    vehicleData->speed *= (1.0 - (vehicleData->altitude / 10000.0));
    
    // Giới hạn tốc độ
    if (vehicleData->speed < 0) vehicleData->speed = 0;
    if (vehicleData->speed > 120) vehicleData->speed = 120;
    
    // Cập nhật thông số động cơ
    vehicleData->engine_power = 150 * power_multiplier; // 150kW là công suất tiêu chuẩn
    vehicleData->engine_torque = 320 * power_multiplier; // 320Nm mô-men xoắn
    
    // Cập nhật nhiệt độ động cơ
    vehicleData->engine_temp = 80.0 + (vehicleData->speed / 2.0);
    
    // Cập nhật pin với các yếu tố ảnh hưởng
    double battery_consumption = vehicleData->speed * 0.001;
    
    // Ảnh hưởng của nhiệt độ lên pin
    if (vehicleData->temperature < 0) {
        battery_consumption *= 1.3; // Tăng 30% tiêu thụ khi nhiệt độ dưới 0°C
    } else if (vehicleData->temperature > 35) {
        battery_consumption *= 1.2; // Tăng 20% tiêu thụ khi nhiệt độ trên 35°C
    }
    
    vehicleData->battery -= battery_consumption;
    if (vehicleData->battery < 0) vehicleData->battery = 0;
    
    // Cập nhật khoảng cách ước tính
    vehicleData->estimated_distance = vehicleData->battery * 6.0;
    
    // Cập nhật khoảng cách đã đi
    vehicleData->distance_traveled += vehicleData->speed / 3600.0;
    
    // Cập nhật hệ thống phanh
    vehicleData->brake_pressure = vehicleData->brake ? 100.0 : 0.0;
    vehicleData->abs_active = vehicleData->brake && (vehicleData->speed > 50);
    vehicleData->esp_active = vehicleData->speed > 80 && vehicleData->wind > 30;
}