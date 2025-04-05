#ifndef BASE_THREAD_HANDLER_H
#define BASE_THREAD_HANDLER_H

#include "IThreadHandler.h"
#include "VehicleData.h"
#include <thread>
#include <atomic>
#include <memory>
#include <stdexcept>
#include "ThreadPriority.h"

/**
 * @class BaseThreadHandler
 * @brief Base class for thread handlers implementing common functionality
 */
class BaseThreadHandler : public IThreadHandler {
protected:
    std::atomic<bool> running;
    std::shared_ptr<VehicleData> vehicleData;
    ThreadPriority priority;

private:
    std::thread thread;

public:
    /**
     * @brief Constructor
     * @param vehicleData Shared vehicle data
     */
    explicit BaseThreadHandler(std::shared_ptr<VehicleData> vehicleData)
        : running(false),
          vehicleData(vehicleData),
          priority(ThreadPriority::NORMAL) {
        if (!vehicleData) {
            throw std::invalid_argument("VehicleData cannot be null");
        }
    }
    
    /**
     * @brief Destructor
     */
    virtual ~BaseThreadHandler() {
        stop();
    }
    
    void setPriority(ThreadPriority newPriority) {
        priority = newPriority;
        if (running && thread.joinable()) {
            #ifdef _WIN32
            SetThreadPriority(reinterpret_cast<HANDLE>(thread.native_handle()), static_cast<int>(priority));
            #endif
        }
    }

    ThreadPriority getPriority() const {
        return priority;
    }

    void start() override {
        if (running) {
            return;
        }
        running = true;
        thread = std::thread(&BaseThreadHandler::threadFunction, this);
        #ifdef _WIN32
        SetThreadPriority(reinterpret_cast<HANDLE>(thread.native_handle()), static_cast<int>(priority));
        #endif
    }
    
    void stop() override {
        if (!running) {
            return;
        }
        running = false;
        if (thread.joinable()) {
            thread.join();
        }
    }
    
    bool isRunning() const override {
        return running;
    }
    
    std::thread::id getThreadId() const override {
        return thread.get_id();
    }


};

#endif // BASE_THREAD_HANDLER_H