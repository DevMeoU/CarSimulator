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
    static int threadID = 0;

    // In ra số hiệu thread chỉ một lần khi bắt đầu
    std::cout << "Keyboard thread #" << threadID << " running" << std::endl;
    std::cout << "Press SPACE to accelerate, ENTER to brake, D to set gear to D, R to set gear to R, "
                 "N to set gear to N, P to set parking, V for left signal, B for right signal" << std::endl;

    while (running.load()) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate);
        
        // Chỉ xử lý input mỗi 50ms (debounce)
        if (elapsed.count() >= 50) {
            lastUpdate = now;
            
            // Lambda để kiểm tra phím với debouncing
            auto checkKey = [&](int key, auto action) {
                bool currentState = (GetAsyncKeyState(key) & 0x8000) != 0;
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
            checkKey('V', [&]() { vehicle.setLeftSignalOn(true); });
            checkKey('B', [&]() { vehicle.setRightSignalOn(true); });
        }
        
        // Chuyển elapsed từ ms sang giây (double)
        double deltaTime = elapsed.count() / 1000.0;
        
        // Giảm tốc theo quán tính nếu không nhấn phím space
        // if (!(GetAsyncKeyState(VK_SPACE) & 0x8000)) {
        //     vehicle.decelerate(0.2 * deltaTime);
        // }
        
        vehicle.update(deltaTime);
        
        // In thông số phương tiện
        std::cout << "Speed: " << vehicle.getSpeed() << " km/h" << std::endl;
        std::cout << "Brake: " << (vehicle.isBrakeActive() ? "Yes" : "No") << std::endl;
        std::cout << "Accelerator: " << (vehicle.isAcceleratorActive() ? "Yes" : "No") << std::endl;
        std::cout << "Left Signal: " << (vehicle.isLeftSignalOn() ? "Yes" : "No") << std::endl;
        std::cout << "Right Signal: " << (vehicle.isRightSignalOn() ? "Yes" : "No") << std::endl;
        std::cout << "Engine Running: " << (vehicle.isEngineRunning() ? "Yes" : "No") << std::endl;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void environment_thread(Vehicle& vehicle) {
    static std::atomic<int> enviromentThreadCount = {0};

    while (running.load()) {
        std::cout << "Environment thread #" << ++enviromentThreadCount << " running" << std::endl;
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

        // Tạm thời in thông số môi trường
        std::cout << "Temperature: " << temperature << " °C" << std::endl;
        std::cout << "Altitude: " << altitude << " m" << std::endl;
        std::cout << "Ambient Temperature: " << env.calculateAmbientTemperature() << " °C" << std::endl;
        std::cout << "Humidity: " << env.getHumidity() << " %" << std::endl;
        std::cout << "Wind Speed: " << env.getWindSpeed() << " m/s" << std::endl;
        std::cout << "Battery Temperature: " << vehicle.getBattery().getTemperature() << " °C" << std::endl;

        // Sleep for a while before checking again
        this_thread::sleep_for(chrono::seconds(5));
    }
}

int main() {
    // Initialize Winsock with error handling
    WSADATA wsaData;
    int wsaResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsaResult != 0) {
        std::cerr << "WSAStartup failed with error: " << wsaResult << std::endl;
        return 1;
    }
    atexit([]() { WSACleanup(); });  // Ensure cleanup on exit

    std::cout << "System Initialization!" << std::endl;

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
        
        // Kiểm tra trạng thái running trước khi khởi tạo
        if (!vehicleData->running.load()) {
            std::cerr << "Vehicle data is not running, cannot start server thread" << std::endl;
            WSACleanup();
            return 1;
        }

        // Declare serverThread outside try block for proper scope
        ServerThread serverThread(vehicleData, "http://localhost:8080");
        
        // Start server thread với xử lý exception chi tiết
        try {
            serverThread.start();
            if (!serverThread.isRunning()) {
                throw std::runtime_error("Server thread failed to start");
            }
            
            std::cout << "Server thread started successfully" << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid server configuration: " << e.what() << std::endl;
            running = false;
            WSACleanup();
            return 1;
        } catch (const std::runtime_error& e) {
            std::cerr << "Server thread runtime error: " << e.what() << std::endl;
            running = false;
            WSACleanup();
            return 1;
        } catch (const std::exception& e) {
            std::cerr << "Server thread initialization failed: " << e.what() << std::endl;
            running = false;
            WSACleanup();
            return 1;
        }

        // Main loop with status checks
        while (vehicleData->running.load()) {
            if (!serverThread.isRunning()) {
                std::cerr << "Server thread stopped unexpectedly" << std::endl;
                vehicleData->running = false;
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        // Graceful shutdown sequence
        try {
            vehicleData->running = false;
            
            // Stop threads with timeout
            auto stopWithTimeout = [](std::thread& t) {
                if (t.joinable()) {
                    if (t.joinable() && t.get_id() != std::this_thread::get_id()) {
                        t.join();
                    }
                }
            };
            
            stopWithTimeout(keyboard_thread);
            stopWithTimeout(env_thread);
            
            serverThread.stop();
            std::cout << "All threads stopped successfully" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error during shutdown: " << e.what() << std::endl;
        }

        WSACleanup();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Exception in main: " << e.what() << std::endl;
        running = false;
        WSACleanup();
        return 1;
    }
    return 0;
}