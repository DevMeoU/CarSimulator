#pragma once

#include <memory>
#include <string>
#include <thread>
#include <mutex>
#include "VehicleData.h"
#include "external/nlohmann/json.hpp"
#include "../../server/cpp-httplib/httplib.h"

using json = nlohmann::json;

class ServerThread {
public:
    ServerThread(std::shared_ptr<VehicleData> data, const std::string& url);
    ~ServerThread();
    
    void start();
    void stop();
    bool ServerThread::isRunning() const;
    
private:
    void threadFunction();
    json prepareData();
    void sendDataToServer(const json& data);
    
    std::shared_ptr<VehicleData> vehicleData;
    std::string serverUrl;
    std::unique_ptr<::httplib::Client> cli;
    std::thread thread;
    bool running;
};