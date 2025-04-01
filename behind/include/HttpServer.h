#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <string>
#include <thread>
#include <atomic>
#include <memory>
#include <functional>
#include "../include/external/cpp-httplib/httplib.h"
#include "../include/DataManager.h"
#include "../include/Vehicle.h"

/**
 * @class HttpServer
 * @brief Implements a HTTP server for the vehicle simulation
 */
class HttpServer {
private:
    Vehicle& vehicle;
    DataManager& dataManager;
    std::string uiPath;
    std::string dataPath;
    int port;
    std::string host;
    
    std::unique_ptr<httplib::Server> server;
    std::thread serverThread;
    std::atomic<bool> running;
    
    /**
     * @brief Thread function for running the server
     */
    void serverThreadFunction();
    
    /**
     * @brief Set up server routes
     */
    void setupRoutes();
    
public:
    /**
     * @brief Constructor
     * @param vehicle Reference to the vehicle instance
     * @param dataManager Reference to the data manager
     * @param uiPath Path to the UI files
     * @param dataPath Path to the data files
     * @param port Server port
     * @param host Server host
     */
    HttpServer(Vehicle& vehicle, DataManager& dataManager, 
               const std::string& uiPath, const std::string& dataPath,
               int port = 8080, const std::string& host = "localhost");
    
    /**
     * @brief Destructor
     */
    ~HttpServer();
    
    /**
     * @brief Start the server
     * @return true if server started successfully, false otherwise
     */
    bool start();
    
    /**
     * @brief Stop the server
     */
    void stop();
    
    /**
     * @brief Check if server is running
     * @return true if running, false otherwise
     */
    bool isRunning() const;
    
    /**
     * @brief Get the server URL
     * @return Server URL
     */
    std::string getServerUrl() const;
};

#endif // HTTP_SERVER_H
