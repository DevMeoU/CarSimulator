#include "../include/DataUpdater.h"
#include <iostream>

DataUpdater::DataUpdater(Vehicle& vehicle, DataManager& dataManager, int updateIntervalMs)
    : vehicle(vehicle), dataManager(dataManager), running(false), 
      updateInterval(std::chrono::milliseconds(updateIntervalMs)) {
}

DataUpdater::~DataUpdater() {
    stop();
}

void DataUpdater::updateThreadFunction() {
    while (running) {
        try {
            // Update vehicle state (simulate real-time changes)
            vehicle.update(updateInterval.count() / 1000.0);
            
            // Save current state to file
            dataManager.saveToFile();
            
            // Notify callbacks with current data
            dataManager.notifyUpdateCallbacks(dataManager.vehicleToJson());
            
        } catch (const std::exception& e) {
            std::cerr << "Error in update thread: " << e.what() << std::endl;
        }
        
        // Sleep for the update interval
        std::this_thread::sleep_for(updateInterval);
    }
}

void DataUpdater::start() {
    if (!running) {
        running = true;
        updateThread = std::thread(&DataUpdater::updateThreadFunction, this);
    }
}

void DataUpdater::stop() {
    if (running) {
        running = false;
        if (updateThread.joinable()) {
            updateThread.join();
        }
    }
}

bool DataUpdater::isRunning() const {
    return running;
}

void DataUpdater::setUpdateInterval(int intervalMs) {
    updateInterval = std::chrono::milliseconds(intervalMs);
}

int DataUpdater::getUpdateInterval() const {
    return static_cast<int>(updateInterval.count());
}

void DataUpdater::forceUpdate() {
    try {
        // Update vehicle state
        vehicle.update(0.1); // Small update to avoid large changes
        
        // Save current state to file
        dataManager.saveToFile();
        
        // Notify callbacks with current data
        dataManager.notifyUpdateCallbacks(dataManager.vehicleToJson());
        
    } catch (const std::exception& e) {
        std::cerr << "Error in force update: " << e.what() << std::endl;
    }
}
