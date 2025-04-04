#ifndef KEYBOARD_THREAD_H
#define KEYBOARD_THREAD_H

#include "VehicleData.h"
#include <thread>
#include <atomic>
#include <memory>
#include <mutex>

class KeyboardThread {
private:
    std::thread thread;
    std::atomic<bool> running;
    std::mutex mutex;
    std::shared_ptr<VehicleData> vehicleData;

public:
    KeyboardThread(std::shared_ptr<VehicleData> data);
    ~KeyboardThread();

    void start();
    void stop();
    void run();
    void threadFunction();
    
    int getKeyPress();
    void processKeyEvent(int key);
};

#endif // KEYBOARD_THREAD_H