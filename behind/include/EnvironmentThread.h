#pragma once

#include <memory>
#include <thread>
#include <atomic>
#include "VehicleData.h"

class EnvironmentThread {
private:
    std::shared_ptr<VehicleData> vehicleData;
    std::thread thread;
    std::atomic<bool> running;
    
    void threadFunction();
    void updateEnvironmentalConditions();
    
public:
    EnvironmentThread(std::shared_ptr<VehicleData> data);
    ~EnvironmentThread();
    
    void start();
    void stop();
};