#ifndef THREAD_HANDLERS_H
#define THREAD_HANDLERS_H

#include "BaseThreadHandler.h"
#include "Vehicle.h"
#include "httplib.h"
#include <chrono>
#include <unordered_map>

/**
 * @class KeyboardHandler
 * @brief Handles keyboard input for vehicle control
 */
class KeyboardHandler : public BaseThreadHandler {
public:
    KeyboardHandler(std::shared_ptr<VehicleData> vehicleData, Vehicle& vehicle)
        : BaseThreadHandler(vehicleData), vehicle(vehicle) {}

protected:
    void threadFunction() override;

private:
    Vehicle& vehicle;
};

/**
 * @class EnvironmentHandler
 * @brief Handles environmental simulation and updates
 */
class EnvironmentHandler : public BaseThreadHandler {
public:
    EnvironmentHandler(std::shared_ptr<VehicleData> vehicleData, Vehicle& vehicle)
        : BaseThreadHandler(vehicleData), vehicle(vehicle) {}

protected:
    void threadFunction() override;

private:
    Vehicle& vehicle;
};

/**
 * @class ServerHandler
 * @brief Handles server communication
 */
class ServerHandler : public BaseThreadHandler {
public:
    ServerHandler(std::shared_ptr<VehicleData> vehicleData, const std::string& host, int port)
        : BaseThreadHandler(vehicleData), host(host), port(port) {
        cli = std::make_unique<httplib::Client>(host.c_str(), port);
        cli->set_connection_timeout(3);
        cli->set_read_timeout(5);
        cli->set_write_timeout(5);
    }

protected:
    void threadFunction() override;

private:
    std::string host;
    int port;
    std::unique_ptr<httplib::Client> cli;
};

#endif // THREAD_HANDLERS_H