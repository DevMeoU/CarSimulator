#ifndef MAIN_THREAD_H
#define MAIN_THREAD_H

#include "VehicleData.h"
#include <thread>
#include <atomic>
#include <memory>
#include <mutex>

class MainThread {
private:
    std::thread thread;
    std::atomic<bool> running;
    std::shared_ptr<VehicleData> vehicleData;

public:
    MainThread(std::shared_ptr<VehicleData> data);
    ~MainThread();

    void start();
    void stop();
    void threadFunction();
    void updateVehicleState();
};

#endif // MAIN_THREAD_H