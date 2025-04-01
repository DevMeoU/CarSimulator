#include "../include/DataManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

DataManager::DataManager(Vehicle& vehicle, const std::string& csvFilePath)
    : vehicle(vehicle), csvFilePath(csvFilePath) {
}

DataManager::~DataManager() {
}

json DataManager::vehicleToJson() {
    std::lock_guard<std::mutex> lock(dataMutex);
    
    json data;
    
    // Basic vehicle data
    data["speed"] = vehicle.getSpeed();
    data["mode"] = vehicle.getDrivingMode().getCurrentModeString();
    data["battery"] = vehicle.getBattery().getCurrentCharge() * 100; // Convert to percentage
    data["battery-temp"] = vehicle.getBattery().getTemperature();
    
    // Determine gear based on vehicle state
    std::string gear = "P"; // Default to Park
    if (vehicle.isEngineRunning()) {
        if (vehicle.getSpeed() > 0) {
            gear = "D"; // Drive
        } else {
            gear = "N"; // Neutral
        }
    }
    data["gear"] = gear;
    
    // Range information
    data["estimated-distance"] = vehicle.getBattery().calculateRemainingRange(
        vehicle.getDrivingMode().getCurrentMode(), 
        vehicle.getEnvironment());
    data["distance-traveled"] = vehicle.getDistanceTraveled();
    
    // Signals and controls
    data["signal-left"] = vehicle.isLeftSignalOn() ? 1 : 0;
    data["signal-right"] = vehicle.isRightSignalOn() ? 1 : 0;
    data["wind"] = vehicle.getEnvironment().getWindSpeed();
    data["air-condition"] = vehicle.getEnvironment().getTemperature();
    data["plug-in"] = vehicle.getBattery().isCharging() ? 1 : 0;
    data["door-lock"] = vehicle.isDoorLocked() ? 1 : 0;
    data["seat-belt"] = vehicle.isSeatbeltOn() ? 1 : 0;
    data["park"] = (gear == "P") ? 1 : 0;
    data["brake"] = vehicle.isBrakeActive() ? 1 : 0;
    data["gas"] = vehicle.isAcceleratorActive() ? 1 : 0;
    
    // Warning level (0-5)
    int warningLevel = 0;
    auto warnings = vehicle.getDisplay().getMessages(MessageType::WARNING);
    warningLevel = std::min(5, static_cast<int>(warnings.size()));
    data["warning"] = warningLevel;
    
    return data;
}

std::string DataManager::vehicleToCsv() {
    json jsonData = vehicleToJson();
    std::unordered_map<std::string, std::string> dataMap;
    
    for (auto& [key, value] : jsonData.items()) {
        dataMap[key] = value.is_string() ? value.get<std::string>() : std::to_string(value.get<double>());
    }
    
    return mapToCsv(dataMap);
}

bool DataManager::updateVehicleFromJson(const json& data) {
    std::lock_guard<std::mutex> lock(dataMutex);
    
    try {
        // Update vehicle state based on JSON data
        if (data.contains("speed")) {
            // We don't directly set speed, but we can adjust acceleration to reach this speed
            double targetSpeed = data["speed"];
            double currentSpeed = vehicle.getSpeed();
            if (targetSpeed > currentSpeed) {
                vehicle.accelerate((targetSpeed - currentSpeed) / 10.0);
            } else if (targetSpeed < currentSpeed) {
                vehicle.brake((currentSpeed - targetSpeed) / 10.0);
            }
        }
        
        if (data.contains("mode")) {
            std::string modeStr = data["mode"];
            DrivingModeType mode = DrivingModeType::NORMAL;
            
            if (modeStr == "sport" || modeStr == "SPORT") {
                mode = DrivingModeType::SPORT;
            } else if (modeStr == "eco" || modeStr == "ECO") {
                mode = DrivingModeType::ECO;
            } else if (modeStr == "snow" || modeStr == "SNOW_OFFROAD") {
                mode = DrivingModeType::SNOW_OFFROAD;
            }
            
            vehicle.changeDrivingMode(mode);
        }
        
        if (data.contains("door-lock")) {
            vehicle.setDoorLocked(data["door-lock"] != 0);
        }
        
        if (data.contains("seat-belt")) {
            vehicle.setSeatbeltOn(data["seat-belt"] != 0);
        }
        
        if (data.contains("gear")) {
            std::string gear = data["gear"];
            if (gear == "P") {
                vehicle.stopEngine();
            } else if (gear == "D" && !vehicle.isEngineRunning()) {
                vehicle.startEngine();
            }
        }
        
        // Notify callbacks about the update
        notifyUpdateCallbacks(data);
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error updating vehicle from JSON: " << e.what() << std::endl;
        return false;
    }
}

bool DataManager::updateVehicleFromCsv(const std::string& csvData) {
    try {
        auto dataMap = parseCsvToMap(csvData);
        json jsonData;
        
        for (const auto& [key, value] : dataMap) {
            // Try to convert to number if possible
            try {
                double numValue = std::stod(value);
                jsonData[key] = numValue;
            } catch (const std::exception&) {
                // If conversion fails, use as string
                jsonData[key] = value;
            }
        }
        
        return updateVehicleFromJson(jsonData);
    } catch (const std::exception& e) {
        std::cerr << "Error updating vehicle from CSV: " << e.what() << std::endl;
        return false;
    }
}

bool DataManager::saveToFile() {
    try {
        std::string csvData = vehicleToCsv();
        std::ofstream file(csvFilePath);
        if (!file.is_open()) {
            std::cerr << "Failed to open CSV file for writing: " << csvFilePath << std::endl;
            return false;
        }
        
        file << csvData;
        file.close();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error saving to file: " << e.what() << std::endl;
        return false;
    }
}

bool DataManager::loadFromFile() {
    try {
        std::ifstream file(csvFilePath);
        if (!file.is_open()) {
            std::cerr << "Failed to open CSV file for reading: " << csvFilePath << std::endl;
            return false;
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        
        return updateVehicleFromCsv(buffer.str());
    } catch (const std::exception& e) {
        std::cerr << "Error loading from file: " << e.what() << std::endl;
        return false;
    }
}

void DataManager::registerUpdateCallback(std::function<void(const json&)> callback) {
    std::lock_guard<std::mutex> lock(dataMutex);
    updateCallbacks.push_back(callback);
}

void DataManager::notifyUpdateCallbacks(const json& data) {
    for (const auto& callback : updateCallbacks) {
        callback(data);
    }
}

std::unordered_map<std::string, std::string> DataManager::parseCsvToMap(const std::string& csvData) {
    std::unordered_map<std::string, std::string> result;
    std::istringstream stream(csvData);
    std::string line;
    
    // Skip header if present
    if (std::getline(stream, line) && line.find("Key, Value") != std::string::npos) {
        // This is a header line, skip it
    } else {
        // This wasn't a header, process it as data
        size_t commaPos = line.find(',');
        if (commaPos != std::string::npos) {
            std::string key = line.substr(0, commaPos);
            std::string value = line.substr(commaPos + 1);
            
            // Trim whitespace
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t,") + 1);
            
            result[key] = value;
        }
    }
    
    // Process remaining lines
    while (std::getline(stream, line)) {
        size_t commaPos = line.find(',');
        if (commaPos != std::string::npos) {
            std::string key = line.substr(0, commaPos);
            std::string value = line.substr(commaPos + 1);
            
            // Trim whitespace
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t,") + 1);
            
            result[key] = value;
        }
    }
    
    return result;
}

std::string DataManager::mapToCsv(const std::unordered_map<std::string, std::string>& dataMap) {
    std::stringstream ss;
    ss << "Key, Value,\n";
    
    // Ensure consistent order of fields
    std::vector<std::string> orderedKeys = {
        "speed", "mode", "battery", "battery-temp", "gear", 
        "estimated-distance", "distance-traveled", "signal-left", 
        "signal-right", "wind", "air-condition", "plug-in", 
        "door-lock", "seat-belt", "park", "brake", "gas", "warning"
    };
    
    for (const auto& key : orderedKeys) {
        if (dataMap.find(key) != dataMap.end()) {
            ss << key << ", " << dataMap.at(key) << ",\n";
        }
    }
    
    // Add any keys that weren't in our ordered list
    for (const auto& [key, value] : dataMap) {
        if (std::find(orderedKeys.begin(), orderedKeys.end(), key) == orderedKeys.end()) {
            ss << key << ", " << value << ",\n";
        }
    }
    
    return ss.str();
}
