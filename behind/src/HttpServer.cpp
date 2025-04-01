#include "../include/HttpServer.h"
#include <iostream>
#include <filesystem>

HttpServer::HttpServer(Vehicle& vehicle, DataManager& dataManager, 
                       const std::string& uiPath, const std::string& dataPath,
                       int port, const std::string& host)
    : vehicle(vehicle), dataManager(dataManager), 
      uiPath(uiPath), dataPath(dataPath),
      port(port), host(host), running(false) {
    
    server = std::make_unique<httplib::Server>();
}

HttpServer::~HttpServer() {
    stop();
}

void HttpServer::setupRoutes() {
    // Mount static file directories
    server->set_mount_point("/", uiPath);
    server->set_mount_point("/data", dataPath);
    
    // Set MIME types
    server->set_file_extension_and_mimetype_mapping("html", "text/html");
    server->set_file_extension_and_mimetype_mapping("css", "text/css");
    server->set_file_extension_and_mimetype_mapping("js", "application/javascript");
    server->set_file_extension_and_mimetype_mapping("csv", "text/plain");
    
    // API endpoint to get vehicle data as JSON
    server->Get("/api/vehicle", [this](const httplib::Request&, httplib::Response& res) {
        json data = dataManager.vehicleToJson();
        res.set_content(data.dump(), "application/json");
    });
    
    // API endpoint to update vehicle data from JSON
    server->Post("/api/vehicle", [this](const httplib::Request& req, httplib::Response& res) {
        if (!req.has_header("Content-Type") || req.get_header_value("Content-Type") != "application/json") {
            res.status = 400;
            res.set_content("Invalid Content-Type", "text/plain");
            return;
        }
        
        try {
            json data = json::parse(req.body);
            bool success = dataManager.updateVehicleFromJson(data);
            
            if (success) {
                res.status = 200;
                res.set_content("Vehicle data updated successfully", "text/plain");
            } else {
                res.status = 500;
                res.set_content("Failed to update vehicle data", "text/plain");
            }
        } catch (const std::exception& e) {
            res.status = 400;
            res.set_content(std::string("Invalid JSON data: ") + e.what(), "text/plain");
        }
    });
    
    // API endpoint to handle form data (similar to update_datas.cpp)
    server->Post("/data", [this](const httplib::Request& req, httplib::Response& res) {
        std::string response = "Data received: ";
        
        try {
            std::unordered_map<std::string, std::string> dataMap;
            for (const auto& p : req.params) {
                dataMap[p.first] = p.second;
                response += p.first + "=" + p.second + "; ";
            }
            
            // Update vehicle from the received data
            dataManager.updateVehicleFromCsv(dataManager.mapToCsv(dataMap));
            
            res.set_content(response, "text/plain");
        } catch (const std::exception& e) {
            res.status = 400;
            res.set_content(std::string("Error processing data: ") + e.what(), "text/plain");
        }
    });
    
    // API endpoint to start/stop the engine
    server->Post("/api/engine", [this](const httplib::Request& req, httplib::Response& res) {
        try {
            json data = json::parse(req.body);
            
            if (data.contains("action")) {
                std::string action = data["action"];
                
                if (action == "start") {
                    bool success = vehicle.startEngine();
                    if (success) {
                        res.set_content("Engine started", "text/plain");
                    } else {
                        res.status = 400;
                        res.set_content("Failed to start engine", "text/plain");
                    }
                } else if (action == "stop") {
                    vehicle.stopEngine();
                    res.set_content("Engine stopped", "text/plain");
                } else {
                    res.status = 400;
                    res.set_content("Invalid action", "text/plain");
                }
            } else {
                res.status = 400;
                res.set_content("Missing action parameter", "text/plain");
            }
        } catch (const std::exception& e) {
            res.status = 400;
            res.set_content(std::string("Invalid JSON data: ") + e.what(), "text/plain");
        }
    });
    
    // API endpoint to change driving mode
    server->Post("/api/mode", [this](const httplib::Request& req, httplib::Response& res) {
        try {
            json data = json::parse(req.body);
            
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
                
                bool success = vehicle.changeDrivingMode(mode);
                if (success) {
                    res.set_content("Driving mode changed to " + modeStr, "text/plain");
                } else {
                    res.status = 400;
                    res.set_content("Failed to change driving mode", "text/plain");
                }
            } else {
                res.status = 400;
                res.set_content("Missing mode parameter", "text/plain");
            }
        } catch (const std::exception& e) {
            res.status = 400;
            res.set_content(std::string("Invalid JSON data: ") + e.what(), "text/plain");
        }
    });
}

void HttpServer::serverThreadFunction() {
    std::cout << "Server running at http://" << host << ":" << port << std::endl;
    
    // Set up server routes
    setupRoutes();
    
    // Start the server
    server->listen(host.c_str(), port);
}

bool HttpServer::start() {
    if (!running) {
        // Create data directory if it doesn't exist
        std::filesystem::create_directories(dataPath);
        
        running = true;
        serverThread = std::thread(&HttpServer::serverThreadFunction, this);
        return true;
    }
    return false;
}

void HttpServer::stop() {
    if (running) {
        running = false;
        server->stop();
        if (serverThread.joinable()) {
            serverThread.join();
        }
    }
}

bool HttpServer::isRunning() const {
    return running;
}

std::string HttpServer::getServerUrl() const {
    return "http://" + host + ":" + std::to_string(port);
}
