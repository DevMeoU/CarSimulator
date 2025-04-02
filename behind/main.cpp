#include <iostream>
#include <atomic>
#include <thread>
#include <winsock2.h>
#include <Windows.h>
#include "include/Vehicle.h"
#include "../server/cpp-httplib/httplib.h"
#include "include/external/nlohmann/json.hpp"
#include "include/ServerThread.h"

using namespace std;

std::atomic<bool> running{true};

void keyboard_handler(Vehicle& vehicle) {
    static std::unordered_map<int, bool> keyStates;
    static auto lastUpdate = std::chrono::steady_clock::now();
    
    while (running) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate);
        
        // Only process inputs every 50ms to debounce
        if (elapsed.count() >= 50) {
            lastUpdate = now;
            
            // Check for key presses with debouncing
            auto checkKey = [&](int key, auto action) {
                bool currentState = GetAsyncKeyState(key) & 0x8000;
                if (currentState && !keyStates[key]) {
                    action();
                }
                keyStates[key] = currentState;
            };
            
            checkKey(VK_SPACE, [&]() { vehicle.accelerate(); });
            checkKey(VK_RETURN, [&]() { vehicle.brake(); });
            checkKey('D', [&]() { vehicle.setGear("D"); });
            checkKey('R', [&]() { vehicle.setGear("R"); });
            checkKey('N', [&]() { vehicle.setGear("N"); });
            checkKey('P', [&]() { vehicle.setParking(true); });
        }
        
        this_thread::sleep_for(chrono::milliseconds(10));
    }
}



void environment_thread(Vehicle& vehicle) {
    while (running) {
        // Giả lập thay đổi môi trường
        EnvironmentalCondition env;
        Sensor tempSensor(SensorType::TEMPERATURE);
        Sensor altSensor(SensorType::ALTITUDE);

        // Lấy giá trị môi trường từ sensor
        double temperature = tempSensor.readValue();
        double altitude = altSensor.readValue();

        // Cập nhật thông số môi trường với độ chính xác
        vehicle.updateEnvironment(
            env.calculateAmbientTemperature(),
            env.getHumidity(),
            env.getWindSpeed(),
            altitude
        );

        // Xử lý nhiệt độ pin theo SAE J2931
        vehicle.getBattery().updateTemperature(
            env.calculateBatteryTempSAEJ2931(temperature, vehicle.getBatteryLoad())
        );

        // Hiệu chỉnh hiệu suất động cơ theo độ cao
        vehicle.getEngine().adjustPerformanceForAltitude(altitude);
        this_thread::sleep_for(chrono::seconds(5));
    }
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Khởi tạo dữ liệu chia sẻ giữa các luồng
    auto vehicleData = std::make_shared<VehicleData>();
    vehicleData->running = true;
    
    // Khởi tạo xe và các luồng
    Vehicle vehicle;
    
    // Luồng xử lý bàn phím
    thread keyboard_thread(keyboard_handler, ref(vehicle));
    
    // Luồng môi trường
    thread env_thread(environment_thread, ref(vehicle));
    
    // Luồng server
    ServerThread serverThread(vehicleData, "http://localhost:8080/data");
    serverThread.start();

    // Chờ các luồng kết thúc
    keyboard_thread.join();
    env_thread.join();
    
    // Dừng luồng server
    vehicleData->running = false;
    serverThread.stop();

    WSACleanup();
    return 0;
}