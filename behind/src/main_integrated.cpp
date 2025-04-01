#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <memory>
#include <filesystem>

#include "Vehicle.h"
#include "DataManager.h"
#include "DataUpdater.h"
#include "HttpServer.h"
#include "Utils.h"

// Function prototypes
void displayMenu();
void runSimulation(Vehicle& vehicle, DataManager& dataManager, DataUpdater& dataUpdater, HttpServer& server);
void testIntegratedSystem(Vehicle& vehicle, DataManager& dataManager, HttpServer& server);

int main() {
    // Create directories if they don't exist
    std::filesystem::create_directories("data");
    std::filesystem::create_directories("ui");
    
    // Create vehicle instance
    Vehicle vehicle;
    
    // Create data manager
    DataManager dataManager(vehicle, "data/storage.csv");
    
    // Create data updater
    DataUpdater dataUpdater(vehicle, dataManager, 1000); // Update every 1 second
    
    // Create HTTP server
    HttpServer server(vehicle, dataManager, "ui", "data", 8080, "localhost");
    
    // Welcome message
    Utils::clearScreen();
    std::cout << "====================================================" << std::endl;
    std::cout << "    Electric Vehicle Simulation System (Updated)    " << std::endl;
    std::cout << "====================================================" << std::endl;
    std::cout << std::endl;
    std::cout << "This program simulates the operation of an electric vehicle" << std::endl;
    std::cout << "with data update and web server functionality." << std::endl;
    std::cout << std::endl;
    
    // Main menu loop
    bool running = true;
    while (running) {
        displayMenu();
        
        std::string choice = Utils::getValidatedInput("Enter your choice (1-4): ", 
            [](const std::string& input) {
                if (input.length() != 1) return false;
                char c = input[0];
                return c >= '1' && c <= '4';
            });
        
        Utils::clearScreen();
        
        switch (choice[0]) {
            case '1':
                runSimulation(vehicle, dataManager, dataUpdater, server);
                break;
            case '2':
                testIntegratedSystem(vehicle, dataManager, server);
                break;
            case '3': {
                // Start server in background
                if (!server.isRunning()) {
                    server.start();
                    std::cout << "Server started at " << server.getServerUrl() << std::endl;
                    std::cout << "Press Enter to stop the server..." << std::endl;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    server.stop();
                    std::cout << "Server stopped" << std::endl;
                } else {
                    std::cout << "Server is already running" << std::endl;
                }
                break;
            }
            case '4':
                std::cout << "Exiting program. Goodbye!" << std::endl;
                running = false;
                break;
        }
        
        if (running) {
            std::cout << std::endl;
            std::cout << "Press Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    
    return 0;
}

void displayMenu() {
    Utils::clearScreen();
    std::cout << "====================================================" << std::endl;
    std::cout << "                      MAIN MENU                     " << std::endl;
    std::cout << "====================================================" << std::endl;
    std::cout << "1. Run Complete Simulation" << std::endl;
    std::cout << "2. Test Integrated System" << std::endl;
    std::cout << "3. Start Web Server" << std::endl;
    std::cout << "4. Exit" << std::endl;
    std::cout << "====================================================" << std::endl;
}

void runSimulation(Vehicle& vehicle, DataManager& dataManager, DataUpdater& dataUpdater, HttpServer& server) {
    std::cout << "====================================================" << std::endl;
    std::cout << "          RUNNING COMPLETE SIMULATION               " << std::endl;
    std::cout << "====================================================" << std::endl;
    std::cout << std::endl;
    
    // Reset vehicle state
    vehicle = Vehicle();
    
    // Start data updater
    dataUpdater.start();
    
    // Start server
    server.start();
    std::cout << "Server started at " << server.getServerUrl() << std::endl;
    
    std::cout << "Starting simulation of a complete drive cycle..." << std::endl;
    std::cout << std::endl;
    
    // Step 1: Prepare vehicle
    std::cout << "Step 1: Preparing vehicle" << std::endl;
    vehicle.setDoorLocked(true);
    std::cout << "Doors locked" << std::endl;
    
    vehicle.setSeatbeltOn(true);
    std::cout << "Seatbelt fastened" << std::endl;
    
    vehicle.pressBrake(3.0);
    std::cout << "Brake pressed for 3 seconds" << std::endl;
    
    bool started = vehicle.startEngine();
    std::cout << "Engine start result: " << (started ? "Success" : "Failed") << std::endl;
    if (!started) {
        std::cout << "Messages: " << vehicle.getDisplay().displayAll() << std::endl;
        std::cout << "Simulation aborted due to engine start failure." << std::endl;
        dataUpdater.stop();
        server.stop();
        return;
    }
    
    // Save current state to CSV
    dataManager.saveToFile();
    std::cout << "Vehicle state saved to CSV" << std::endl;
    
    std::cout << std::endl;
    
    // Step 2: City driving in ECO mode
    std::cout << "Step 2: City driving in ECO mode" << std::endl;
    vehicle.changeDrivingMode(DrivingModeType::ECO);
    std::cout << "Changed to ECO mode" << std::endl;
    
    std::cout << "Accelerating to city speed..." << std::endl;
    for (int i = 0; i < 5; i++) {
        vehicle.accelerate(0.3);
        vehicle.update(1.0);
        std::cout << "Speed: " << vehicle.getSpeed() << " km/h" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    // Save current state to CSV
    dataManager.saveToFile();
    
    std::cout << "Cruising in the city..." << std::endl;
    for (int i = 0; i < 10; i++) {
        vehicle.update(1.0);
        
        // Occasional acceleration and braking to simulate city traffic
        if (i % 3 == 0) {
            vehicle.brake(0.2);
            std::cout << "Light braking, speed: " << vehicle.getSpeed() << " km/h" << std::endl;
        } else if (i % 4 == 0) {
            vehicle.accelerate(0.2);
            std::cout << "Light acceleration, speed: " << vehicle.getSpeed() << " km/h" << std::endl;
        } else {
            std::cout << "Cruising, speed: " << vehicle.getSpeed() << " km/h" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    // Save current state to CSV
    dataManager.saveToFile();
    
    std::cout << std::endl;
    
    // Step 3: Highway driving in SPORT mode
    std::cout << "Step 3: Highway driving in SPORT mode" << std::endl;
    vehicle.changeDrivingMode(DrivingModeType::SPORT);
    std::cout << "Changed to SPORT mode" << std::endl;
    
    std::cout << "Accelerating to highway speed..." << std::endl;
    for (int i = 0; i < 10; i++) {
        vehicle.accelerate(0.8);
        vehicle.update(1.0);
        std::cout << "Speed: " << vehicle.getSpeed() << " km/h" << std::endl;
        
        // Check for high speed warning
        auto warnings = vehicle.getDisplay().getMessages(MessageType::WARNING);
        if (!warnings.empty()) {
            std::cout << "Warning triggered: " << vehicle.getDisplay().displayByType(MessageType::WARNING) << std::endl;
            vehicle.getDisplay().clearMessages();
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    // Save current state to CSV
    dataManager.saveToFile();
    
    std::cout << "Cruising on highway..." << std::endl;
    for (int i = 0; i < 10; i++) {
        vehicle.update(1.0);
        std::cout << "Cruising, speed: " << vehicle.getSpeed() << " km/h" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    // Save current state to CSV
    dataManager.saveToFile();
    
    std::cout << std::endl;
    
    // Step 4: Changing environmental conditions
    std::cout << "Step 4: Weather changes - entering rain" << std::endl;
    vehicle.getEnvironment().setWeather(WeatherType::RAIN);
    vehicle.getEnvironment().setTemperature(15.0);
    std::cout << "Weather changed to rain, temperature dropped to 15°C" << std::endl;
    
    std::cout << "Adjusting driving..." << std::endl;
    vehicle.changeDrivingMode(DrivingModeType::NORMAL);
    std::cout << "Changed to NORMAL mode" << std::endl;
    
    for (int i = 0; i < 5; i++) {
        vehicle.brake(0.3);
        vehicle.update(1.0);
        std::cout << "Reducing speed, current speed: " << vehicle.getSpeed() << " km/h" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    // Save current state to CSV
    dataManager.saveToFile();
    
    std::cout << std::endl;
    
    // Step 5: Return to normal conditions and stop
    std::cout << "Step 5: Returning to normal conditions and stopping" << std::endl;
    vehicle.getEnvironment().setWeather(WeatherType::CLEAR);
    vehicle.getEnvironment().setTemperature(20.0);
    std::cout << "Returned to normal road conditions" << std::endl;
    
    std::cout << "Slowing down to stop..." << std::endl;
    while (vehicle.getSpeed() > 0.1) {
        vehicle.brake(0.5);
        vehicle.update(1.0);
        std::cout << "Braking, speed: " << vehicle.getSpeed() << " km/h" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    vehicle.stopEngine();
    std::cout << "Vehicle stopped, engine turned off" << std::endl;
    
    // Save final state to CSV
    dataManager.saveToFile();
    
    // Final status
    std::cout << std::endl;
    std::cout << "Simulation completed. Final vehicle status:" << std::endl;
    std::cout << vehicle.getStatusString() << std::endl;
    std::cout << "Battery status:" << std::endl;
    std::cout << vehicle.getBattery().getStatusString() << std::endl;
    
    // Stop data updater and server
    dataUpdater.stop();
    server.stop();
    std::cout << "Data updater and server stopped" << std::endl;
}

void testIntegratedSystem(Vehicle& vehicle, DataManager& dataManager, HttpServer& server) {
    std::cout << "====================================================" << std::endl;
    std::cout << "          TESTING INTEGRATED SYSTEM                 " << std::endl;
    std::cout << "====================================================" << std::endl;
    std::cout << std::endl;
    
    // Reset vehicle state
    vehicle = Vehicle();
    
    // Test 1: Convert vehicle state to JSON
    std::cout << "Test 1: Convert vehicle state to JSON" << std::endl;
    json vehicleJson = dataManager.vehicleToJson();
    std::cout << "Vehicle JSON: " << vehicleJson.dump(2) << std::endl;
    std::cout << std::endl;
    
    // Test 2: Convert vehicle state to CSV
    std::cout << "Test 2: Convert vehicle state to CSV" << std::endl;
    std::string vehicleCsv = dataManager.vehicleToCsv();
    std::cout << "Vehicle CSV:" << std::endl << vehicleCsv << std::endl;
    
    // Test 3: Save vehicle state to file
    std::cout << "Test 3: Save vehicle state to file" << std::endl;
    bool saveResult = dataManager.saveToFile();
    std::cout << "Save result: " << (saveResult ? "Success" : "Failed") << std::endl;
    std::cout << std::endl;
    
    // Test 4: Update vehicle state from JSON
    std::cout << "Test 4: Update vehicle state from JSON" << std::endl;
    json updateJson = {
        {"speed", 50.0},
        {"mode", "sport"},
        {"door-lock", 1},
        {"seat-belt", 1}
    };
    std::cout << "Update JSON: " << updateJson.dump(2) << std::endl;
    bool updateResult = dataManager.updateVehicleFromJson(updateJson);
    std::cout << "Update result: " << (updateResult ? "Success" : "Failed") << std::endl;
    std::cout << "Updated vehicle state:" << std::endl;
    std::cout << vehicle.getStatusString() << std::endl;
    std::cout << std::endl;
    
    // Test 5: Start HTTP server
    std::cout << "Test 5: Start HTTP server" << std::endl;
    bool serverResult = server.start();
    std::cout << "Server start result: " << (serverResult ? "Success" : "Failed") << std::endl;
    if (serverResult) {
        std::cout << "Server URL: " << server.getServerUrl() << std::endl;
        std::cout << "Available endpoints:" << std::endl;
        std::cout << "  GET  /api/vehicle - Get vehicle data as JSON" << std::endl;
        std::cout << "  POST /api/vehicle - Update vehicle data from JSON" << std::endl;
        std::cout << "  POST /api/engine  - Start/stop engine" << std::endl;
        std::cout << "  POST /api/mode    - Change driving mode" << std::endl;
        std::cout << "  POST /data        - Update vehicle data from form data" << std::endl;
        std::cout << std::endl;
        
        std::cout << "Server is running. Press Enter to stop..." << std::endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        server.stop();
        std::cout << "Server stopped" << std::endl;
    }
    
    std::cout << std::endl;
    std::cout << "Integrated system test completed." << std::endl;
}
