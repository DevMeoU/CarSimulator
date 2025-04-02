#ifdef _WIN32
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0600  // Windows Vista trở lên
    #endif
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <Windows.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netdb.h>
#endif
#include <iostream>
#include <atomic>
#include <thread>
#include "Vehicle.h"
#include "httplib.h"
#include "json.hpp"
#include "ServerThread.h"

using namespace std;

std::atomic<bool> running{true};

void keyboard_handler(Vehicle& vehicle) {
    static std::unordered_map<int, bool> keyStates;
    static auto lastUpdate = std::chrono::steady_clock::now();
    
    while (running.load()) {
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
            
            checkKey(VK_SPACE, [&]() { vehicle.accelerate(0.5); });
            checkKey(VK_RETURN, [&]() { vehicle.brake(0.5); });
            checkKey('D', [&]() { vehicle.setGear("D"); });
            checkKey('R', [&]() { vehicle.setGear("R"); });
            checkKey('N', [&]() { vehicle.setGear("N"); });
            checkKey('P', [&]() { vehicle.setParking(true); });
        }
        
        this_thread::sleep_for(chrono::milliseconds(10));
    }
}



void environment_thread(Vehicle& vehicle) {
    while (running.load()) {
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
            env.calculateBatteryTempSAEJ2931(temperature, vehicle.getBatteryLoad()),
            std::chrono::system_clock::now()
        );

        // Hiệu chỉnh hiệu suất động cơ theo độ cao
        vehicle.getEngine().adjustPerformanceForAltitude(altitude);
        this_thread::sleep_for(chrono::seconds(5));
    }
}

int main() {
    // Initialize Winsock
    WSADATA wsaData;
    int wsaResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsaResult != 0) {
        std::cerr << "WSAStartup failed: " << wsaResult << std::endl;
        return 1;
    }

    try {
        // Initialize shared data between threads
        auto vehicleData = std::make_shared<VehicleData>();
        vehicleData->running = true;
        
        // Initialize vehicle
        Vehicle vehicle;
        
        // Start keyboard thread with error handling
        thread keyboard_thread;
        try {
            keyboard_thread = thread(keyboard_handler, ref(vehicle));
        } catch (const std::system_error& e) {
            std::cerr << "Failed to start keyboard thread: " << e.what() << std::endl;
            WSACleanup();
            return 1;
        }
        
        // Start environment thread with error handling
        thread env_thread;
        try {
            env_thread = thread(environment_thread, ref(vehicle));
        } catch (const std::system_error& e) {
            std::cerr << "Failed to start environment thread: " << e.what() << std::endl;
            running = false;
            keyboard_thread.join();
            WSACleanup();
            return 1;
        }
        
        // Start server thread
        ServerThread serverThread(vehicleData, "http://localhost:8080/data");
        if (!serverThread.isRunning() || !vehicleData->running.load()) {
            std::cerr << "Failed to start server thread" << std::endl;
            running = false;
            keyboard_thread.join();
            env_thread.join();
            WSACleanup();
            return 1;
        }

        // Wait for threads to finish
        keyboard_thread.join();
        env_thread.join();
        
        // Stop server thread
        vehicleData->running = false;
        serverThread.stop();

        WSACleanup();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Exception in main: " << e.what() << std::endl;
        running = false;
        WSACleanup();
        return 1;
    }
}