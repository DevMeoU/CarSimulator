#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <mutex>
#include "../include/external/json/json.hpp"
#include "../include/Vehicle.h"

using json = nlohmann::json;

/**
 * @class DataManager
 * @brief Manages data conversion and storage for the vehicle simulation
 */
class DataManager {
private:
    Vehicle& vehicle;
    std::string csvFilePath;
    std::mutex dataMutex;
    
    // Callback for data updates
    std::vector<std::function<void(const json&)>> updateCallbacks;
    
public:
    /**
     * @brief Constructor
     * @param vehicle Reference to the vehicle instance
     * @param csvFilePath Path to the CSV file for data storage
     */
    DataManager(Vehicle& vehicle, const std::string& csvFilePath);
    
    /**
     * @brief Destructor
     */
    ~DataManager();
    
    /**
     * @brief Convert vehicle state to JSON
     * @return JSON object representing vehicle state
     */
    json vehicleToJson();
    
    /**
     * @brief Convert vehicle state to CSV format
     * @return CSV string representing vehicle state
     */
    std::string vehicleToCsv();
    
    /**
     * @brief Update vehicle state from JSON data
     * @param data JSON object with vehicle state data
     * @return true if update was successful, false otherwise
     */
    bool updateVehicleFromJson(const json& data);
    
    /**
     * @brief Update vehicle state from CSV data
     * @param csvData CSV string with vehicle state data
     * @return true if update was successful, false otherwise
     */
    bool updateVehicleFromCsv(const std::string& csvData);
    
    /**
     * @brief Save vehicle state to CSV file
     * @return true if save was successful, false otherwise
     */
    bool saveToFile();
    
    /**
     * @brief Load vehicle state from CSV file
     * @return true if load was successful, false otherwise
     */
    bool loadFromFile();
    
    /**
     * @brief Register callback for data updates
     * @param callback Function to call when data is updated
     */
    void registerUpdateCallback(std::function<void(const json&)> callback);
    
    /**
     * @brief Notify all registered callbacks about data update
     * @param data Updated JSON data
     */
    void notifyUpdateCallbacks(const json& data);
    
    /**
     * @brief Parse CSV string to key-value map
     * @param csvData CSV string to parse
     * @return Map of keys to values
     */
    std::unordered_map<std::string, std::string> parseCsvToMap(const std::string& csvData);
    
    /**
     * @brief Convert key-value map to CSV string
     * @param dataMap Map of keys to values
     * @return CSV string
     */
    std::string mapToCsv(const std::unordered_map<std::string, std::string>& dataMap);
};

#endif // DATA_MANAGER_H
