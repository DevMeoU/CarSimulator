#pragma once

#include <memory>
#include <string>
#include <thread>
#include <mutex>
#include "VehicleData.h"
#include "json.hpp"
#include "httplib.h"

using json = nlohmann::json;

class ServerThread {
public:
    ServerThread(std::shared_ptr<VehicleData> data, const std::string& url);
    ~ServerThread();
    
    void start();
    void stop();
    bool isRunning() const;
    bool isConnected() const;
    
private:
    void threadFunction();
    json prepareData();
    bool sendWithRetry(const json& data, int maxRetries, int retryIntervalMs);
    
    std::shared_ptr<VehicleData> vehicleData;
    std::string serverUrl;
    std::unique_ptr<::httplib::Client> cli;
    std::thread thread;
    bool running;
    bool connectionStatus;
    std::string host;
    int port;
};