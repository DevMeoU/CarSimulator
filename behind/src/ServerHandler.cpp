#include "ThreadHandlers.h"
#include "json.hpp"
#include <iostream>

void ServerHandler::threadFunction() {
    std::cout << "[Server] Thread started" << std::endl;
    
    const int RETRY_INTERVAL_MS = 1000;

    while (running) {
        try {
            if (!cli->is_socket_open()) {
                std::cout << "[Server] Attempting to connect to " << host << ":" << port << std::endl;
                cli = std::make_unique<httplib::Client>(host.c_str(), port);
                cli->set_connection_timeout(3);
                cli->set_read_timeout(5);
            }

            nlohmann::json data = {
                {"speed", vehicleData->speed},
                {"brake", vehicleData->brake},
                {"gas", vehicleData->gas},
                {"signal_left", vehicleData->signal_left},
                {"signal_right", vehicleData->signal_right},
                {"engine_power", vehicleData->engine_power},
                {"engine_temp", vehicleData->engine_temp},
                {"battery_temp", vehicleData->battery_temp}
            };

            std::cout << "[Server] Sending vehicle data..." << std::endl;
            auto res = cli->Post("/api/data", data.dump(), "application/json");
            
            if (res && res->status == 200) {
                std::cout << "[Server] Data sent successfully" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            } else {
                std::cout << "[Server] Failed to send data" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(RETRY_INTERVAL_MS));
            }
        } catch (const std::exception& e) {
            std::cout << "[Server] Communication error: " << e.what() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(RETRY_INTERVAL_MS));
        }
    }
    
    std::cout << "[Server] Thread stopped" << std::endl;
}