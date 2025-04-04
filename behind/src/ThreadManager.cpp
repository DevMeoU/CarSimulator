#include "ThreadManager.h"
#include <stdexcept>
#include <iostream>

ThreadManager::ThreadManager(std::shared_ptr<VehicleData> vehicleData)
    : vehicleData(vehicleData) {
    if (!vehicleData) {
        throw std::invalid_argument("VehicleData cannot be null");
    }
}

ThreadManager::~ThreadManager() {
    try {
        stopAll();
    } catch (const std::exception& e) {
        std::cerr << "Error during ThreadManager destruction: " << e.what() << std::endl;
    }
}

void ThreadManager::addHandler(const std::string& name, std::shared_ptr<IThreadHandler> handler) {
    if (name.empty()) {
        throw std::invalid_argument("Thread name cannot be empty");
    }
    if (!handler) {
        throw std::invalid_argument("Thread handler cannot be null");
    }

    std::lock_guard<std::mutex> lock(mutex);
    if (handlers.find(name) != handlers.end()) {
        throw std::runtime_error("Thread handler with name '" + name + "' already exists");
    }
    handlers[name] = handler;
}

bool ThreadManager::startAll() {
    std::lock_guard<std::mutex> lock(mutex);
    bool allStarted = true;

    for (auto& pair : handlers) {
        try {
            if (!pair.second->isRunning()) {
                pair.second->start();
                if (!pair.second->isRunning()) {
                    std::cerr << "Failed to start thread: " << pair.first << std::endl;
                    allStarted = false;
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Error starting thread '" << pair.first << "': " << e.what() << std::endl;
            allStarted = false;
        }
    }

    return allStarted;
}

void ThreadManager::stopAll() {
    std::lock_guard<std::mutex> lock(mutex);

    for (auto& pair : handlers) {
        try {
            if (pair.second->isRunning()) {
                pair.second->stop();
            }
        } catch (const std::exception& e) {
            std::cerr << "Error stopping thread '" << pair.first << "': " << e.what() << std::endl;
        }
    }
}

std::shared_ptr<IThreadHandler> ThreadManager::getHandler(const std::string& name) const {
    std::lock_guard<std::mutex> lock(mutex);
    auto it = handlers.find(name);
    return (it != handlers.end()) ? it->second : nullptr;
}

bool ThreadManager::areAllThreadsRunning() const {
    std::lock_guard<std::mutex> lock(mutex);
    
    if (handlers.empty()) {
        return false;
    }

    for (const auto& pair : handlers) {
        if (!pair.second->isRunning()) {
            return false;
        }
    }
    return true;
}

size_t ThreadManager::getRunningThreadCount() const {
    std::lock_guard<std::mutex> lock(mutex);
    size_t count = 0;

    for (const auto& pair : handlers) {
        if (pair.second->isRunning()) {
            count++;
        }
    }
    return count;
}