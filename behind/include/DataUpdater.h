#ifndef DATA_UPDATER_H
#define DATA_UPDATER_H

#include <string>
#include <thread>
#include <atomic>
#include <functional>
#include <chrono>
#include "../include/DataManager.h"
#include "../include/Vehicle.h"

/**
 * @class DataUpdater
 * @brief Manages periodic updates of vehicle data
 */
class DataUpdater {
private:
    Vehicle& vehicle;
    DataManager& dataManager;
    std::thread updateThread;
    std::atomic<bool> running;
    std::chrono::milliseconds updateInterval;
    
    /**
     * @brief Thread function for periodic updates
     */
    void updateThreadFunction();
    
public:
    /**
     * @brief Constructor
     * @param vehicle Reference to the vehicle instance
     * @param dataManager Reference to the data manager
     * @param updateIntervalMs Update interval in milliseconds
     */
    DataUpdater(Vehicle& vehicle, DataManager& dataManager, int updateIntervalMs = 1000);
    
    /**
     * @brief Destructor
     */
    ~DataUpdater();
    
    /**
     * @brief Start the update thread
     */
    void start();
    
    /**
     * @brief Stop the update thread
     */
    void stop();
    
    /**
     * @brief Check if updater is running
     * @return true if running, false otherwise
     */
    bool isRunning() const;
    
    /**
     * @brief Set the update interval
     * @param intervalMs Interval in milliseconds
     */
    void setUpdateInterval(int intervalMs);
    
    /**
     * @brief Get the current update interval
     * @return Interval in milliseconds
     */
    int getUpdateInterval() const;
    
    /**
     * @brief Force an immediate update
     */
    void forceUpdate();
};

#endif // DATA_UPDATER_H
