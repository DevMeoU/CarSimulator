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
#include <memory>
#include <stdexcept>
#include "Vehicle.h"
#include "httplib.h"
#include "json.hpp"
#include "ThreadManager.h"
#include "ThreadHandlers.h"
#include "VehicleData.h"

using namespace std;

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

    // Initialize shared data and vehicle
    auto vehicleData = std::make_shared<VehicleData>();
    vehicleData->running = true;
    Vehicle vehicle;

    try {

        // Initialize thread manager
        ThreadManager threadManager(vehicleData);

        // Create thread handlers
        auto keyboardHandler = std::make_shared<KeyboardHandler>(vehicleData, vehicle);
        auto environmentHandler = std::make_shared<EnvironmentHandler>(vehicleData, vehicle);
        auto serverHandler = std::make_shared<ServerHandler>(vehicleData, "localhost", 8080);

        // Add handlers to manager with priorities
        threadManager.addHandler("keyboard", keyboardHandler);
        threadManager.addHandler("environment", environmentHandler);
        threadManager.addHandler("server", serverHandler);

        // Set thread priorities
        threadManager.setThreadPriority("keyboard", ThreadPriority::HIGHEST);      // Độ ưu tiên cao nhất cho xử lý phím
        threadManager.setThreadPriority("server", ThreadPriority::ABOVE_NORMAL);   // Độ ưu tiên trên trung bình cho gửi dữ liệu
        threadManager.setThreadPriority("environment", ThreadPriority::LOWEST);     // Độ ưu tiên thấp nhất cho môi trường

        // Start all threads
        if (!threadManager.startAll()) {
            throw std::runtime_error("Failed to start all threads");
        }

        std::cout << "All threads started successfully" << std::endl;
        std::cout << "Press SPACE to accelerate, ENTER to brake, D for Drive, R for Reverse,\n"
                  << "N for Neutral, P for Park, V for left signal, B for right signal" << std::endl;

        // Main loop
        while (vehicleData->running.load()) {
            if (!threadManager.areAllThreadsRunning()) {
                std::cerr << "One or more threads stopped unexpectedly" << std::endl;
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        // Graceful shutdown
        threadManager.stopAll();
        std::cout << "All threads stopped successfully" << std::endl;

        WSACleanup();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Exception in main: " << e.what() << std::endl;
        vehicleData->running = false;
        WSACleanup();
        return 1;
    }
}