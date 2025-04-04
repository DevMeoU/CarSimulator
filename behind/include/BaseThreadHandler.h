#ifndef BASE_THREAD_HANDLER_H
#define BASE_THREAD_HANDLER_H

#include "IThreadHandler.h"
#include "VehicleData.h"
#include <thread>
#include <atomic>
#include <memory>
#include <stdexcept>

/**
 * @class BaseThreadHandler
 * @brief Base class for thread handlers implementing common functionality
 */
class BaseThreadHandler : public IThreadHandler {
public:
    /**
     * @brief Constructor
     * @param vehicleData Shared vehicle data
     */
    explicit BaseThreadHandler(std::shared_ptr<VehicleData> vehicleData)
        : vehicleData(vehicleData), running(false) {
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
    
    void start() override {
        if (running) {
            return;
        }
        running = true;
        thread = std::thread(&BaseThreadHandler::threadFunction, this);
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

protected:
    std::shared_ptr<VehicleData> vehicleData;
    std::atomic<bool> running;
    std::thread thread;
};

#endif // BASE_THREAD_HANDLER_H