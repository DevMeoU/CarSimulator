#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#include "../include/ServerThread.h"
#include "../include/VehicleData.h"
#include <thread>
#include <chrono>

#include "../../server/cpp-httplib/httplib.h"
#include "../include/external/nlohmann/json.hpp"

using json = nlohmann::json;

ServerThread::ServerThread(std::shared_ptr<VehicleData> data, const std::string& url)
    : vehicleData(data), serverUrl(url), running(false) {}

void ServerThread::start() {
    running = true;
    thread = std::thread(&ServerThread::threadFunction, this);
}

void ServerThread::stop() {
    running = false;
    if (thread.joinable()) {
        thread.join();
    }
}

void ServerThread::threadFunction() {
    while (running) {
        json data = prepareData();
        sendDataToServer(data);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

json ServerThread::prepareData() {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    
    json data;
    data["speed"] = vehicleData->speed;
    data["mode"] = vehicleData->mode;
    data["battery"] = vehicleData->battery;
    data["battery_temp"] = vehicleData->battery_temp;
    data["gear"] = vehicleData->gear;
    data["estimated_distance"] = vehicleData->estimated_distance;
    data["distance_traveled"] = vehicleData->distance_traveled;
    data["signal_left"] = vehicleData->signal_left;
    data["signal_right"] = vehicleData->signal_right;
    data["wind"] = vehicleData->wind;
    data["air_condition"] = vehicleData->air_condition;
    data["plug_in"] = vehicleData->plug_in;
    data["door_lock"] = vehicleData->door_lock;
    data["seat_belt"] = vehicleData->seat_belt;
    data["park"] = vehicleData->park;
    data["brake"] = vehicleData->brake;
    data["gas"] = vehicleData->gas;
    data["warning"] = vehicleData->warning;
    data["temperature"] = vehicleData->temperature;
    data["altitude"] = vehicleData->altitude;
    data["weather"] = vehicleData->weather;
    data["engine_power"] = vehicleData->engine_power;
    data["engine_torque"] = vehicleData->engine_torque;
    data["engine_temp"] = vehicleData->engine_temp;
    data["brake_pressure"] = vehicleData->brake_pressure;
    data["abs_active"] = vehicleData->abs_active;
    data["esp_active"] = vehicleData->esp_active;
    

    
    return data;
}

void ServerThread::sendDataToServer(const json& data) {
    httplib::Client cli(serverUrl);
    
    auto res = cli.Post("/data", data.dump(), "application/json");
    
    if (!res || res->status != 200) {
        std::cerr << "Failed to send data to server" << std::endl;
    }
}