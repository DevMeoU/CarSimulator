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
                {"abs_active", false},
                {"air_condition", vehicleData->air_condition},
                {"altitude", vehicleData->altitude},
                {"battery", vehicleData->battery},
                {"battery_temp", vehicleData->battery_temp},
                {"brake", vehicleData->brake},
                {"brake_pressure", vehicleData->brake_pressure},
                {"distance_traveled", vehicleData->distance_traveled},
                {"door_lock", vehicleData->door_lock},
                {"engine_power", vehicleData->engine_power},
                {"engine_temp", vehicleData->engine_temp},
                {"engine_torque", vehicleData->engine_torque},
                {"esp_active", false},
                {"estimated_distance", vehicleData->estimated_distance},
                {"gas", vehicleData->gas},
                {"gear", vehicleData->gear},
                {"mode", vehicleData->mode},
                {"park", vehicleData->park},
                {"plug_in", vehicleData->plug_in},
                {"seat_belt", vehicleData->seat_belt},
                {"signal_left", vehicleData->signal_left},
                {"signal_right", vehicleData->signal_right},
                {"speed", vehicleData->speed},
                {"temperature", vehicleData->temperature},
                {"timestamp", std::to_string(vehicleData->timestamp_ms)},
                // {"warning", vehicleData->warning},
                {"Speed", vehicleData->speed},
                {"Distance", vehicleData->distance_traveled},
                {"Battery", vehicleData->battery},
                {"Engine", "Running"},
                {"Doors", vehicleData->door_lock ? "Locked" : "Unlocked"},
                {"Seatbelt", vehicleData->seat_belt ? "On" : "Off"},
                {"weather", vehicleData->weather},
                {"wind", vehicleData->wind}
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