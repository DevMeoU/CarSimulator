#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <mutex>
#include "IThreadHandler.h"
#include "VehicleData.h"

/**
 * @class ThreadManager
 * @brief Manages thread initialization and control
 */
class ThreadManager {
public:
    /**
     * @brief Constructor
     * @param vehicleData Shared vehicle data
     */
    explicit ThreadManager(std::shared_ptr<VehicleData> vehicleData);
    
    /**
     * @brief Destructor
     */
    ~ThreadManager();
    
    /**
     * @brief Add a thread handler
     * @param name Thread name
     * @param handler Thread handler
     */
    void addHandler(const std::string& name, std::shared_ptr<IThreadHandler> handler);
    
    /**
     * @brief Start all threads
     * @return true if all threads started successfully
     */
    bool startAll();
    
    /**
     * @brief Stop all threads
     */
    void stopAll();
    
    /**
     * @brief Get thread handler by name
     * @param name Thread name
     * @return Thread handler or nullptr if not found
     */
    std::shared_ptr<IThreadHandler> getHandler(const std::string& name) const;
    
    /**
     * @brief Check if all threads are running
     * @return true if all threads are running
     */
    bool areAllThreadsRunning() const;
    
    /**
     * @brief Get number of running threads
     * @return Number of running threads
     */
    size_t getRunningThreadCount() const;

private:
    std::shared_ptr<VehicleData> vehicleData;
    std::unordered_map<std::string, std::shared_ptr<IThreadHandler>> handlers;
    mutable std::mutex mutex;
};

#endif // THREAD_MANAGER_H