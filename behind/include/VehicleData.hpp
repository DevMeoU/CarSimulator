#pragma once

#include <string>
#include <mutex>
#include <atomic>

struct VehicleData {
    std::mutex mutex;
    std::atomic<bool> running;
    
    // Thông số xe
    double speed;
    std::string mode;
    double battery;
    double battery_temp;
    std::string gear;
    double estimated_distance;
    double distance_traveled;
    
    // Trạng thái xe
    bool signal_left;
    bool signal_right;
    bool plug_in;
    bool door_lock;
    bool seat_belt;
    bool park;
    bool brake;
    bool gas;
    
    // Thông số môi trường
    double wind;
    double air_condition;
    double temperature;
    double altitude;
    std::string weather;
    
    // Cảnh báo
    std::string warning;
    
    // Thông số động cơ
    double engine_power;
    double engine_torque;
    double engine_temp;
    
    // Thông số phanh
    double brake_pressure;
    bool abs_active;
    bool esp_active;
};