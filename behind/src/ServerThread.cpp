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

#include "ServerThread.h"
#include "VehicleData.h"
#include <thread>
#include <chrono>

#include <httplib.h>
#include "json.hpp"

using json = nlohmann::json;

ServerThread::ServerThread(std::shared_ptr<VehicleData> data, const std::string& url)
    : vehicleData(data), running(false) {
    size_t pos = url.find(':');
    std::string host = url.substr(0, pos);
    int port = std::stoi(url.substr(pos + 1));
    cli = std::make_unique<::httplib::Client>(host, port);
    cli->set_connection_timeout(2);
    cli->set_read_timeout(2);
}

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
    if(!cli || !cli->is_socket_open()) {
        std::cerr << "Server connection failed" << std::endl;
        return;
    }

    auto res = cli->Post("/data", data.dump(), "application/json");

    if (!res) {
        std::cerr << "Network error: " << httplib::to_string(res.error()) << std::endl;
    } else if (res->status != 200) {
        std::cerr << "Server returned HTTP error: " << res->status << std::endl;
    }
}

bool ServerThread::isRunning() const {
    return running;
}

ServerThread::~ServerThread() {
    stop();
}