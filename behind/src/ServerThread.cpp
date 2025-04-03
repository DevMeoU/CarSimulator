#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif

    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0600
    #endif

    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>
#else
    #include <sys/socket.h>
    #include <netdb.h>
    #include <arpa/inet.h>
#endif

#include "ServerThread.h"
#include "VehicleData.h"
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <sstream>
#include <iomanip>

using json = nlohmann::json;

// Logger helper class
class Logger {
public:
    static void log(const std::string& message) {
        auto now = std::chrono::system_clock::now();
        auto now_time = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S") << " - " << message;
        std::cout << ss.str() << std::endl;
    }

    static void error(const std::string& message) {
        log("[ERROR] " + message);
    }

    static void warning(const std::string& message) {
        log("[WARNING] " + message);
    }

    static void info(const std::string& message) {
        log("[INFO] " + message);
    }
};

ServerThread::ServerThread(std::shared_ptr<VehicleData> data, const std::string& url)
    : vehicleData(data), running(false), connectionStatus(false) {
    try {
#ifdef _WIN32
        // Initialize Winsock
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            throw std::runtime_error("Winsock initialization failed");
        }
        Logger::info("Winsock initialized successfully");
#endif

        // Validate and parse URL
        if (url.empty()) {
            throw std::invalid_argument("URL cannot be empty");
        }

        size_t protocol_pos = url.find("://");
        if (protocol_pos == std::string::npos) {
            throw std::invalid_argument("Invalid URL format - missing protocol");
        }

        size_t host_start = protocol_pos + 3;
        size_t port_pos = url.find(':', host_start);
        if (port_pos == std::string::npos) {
            throw std::invalid_argument("Invalid URL format - missing port");
        }

        std::string host = url.substr(host_start, port_pos - host_start);
        size_t path_pos = url.find('/', port_pos);

        std::string port_str;
        if (path_pos != std::string::npos) {
            port_str = url.substr(port_pos + 1, path_pos - port_pos - 1);
        } else {
            port_str = url.substr(port_pos + 1);
        }

        // Validate port number
        if (port_str.empty()) {
            throw std::invalid_argument("Port number cannot be empty");
        }

        for (char c : port_str) {
            if (!isdigit(c)) {
                throw std::invalid_argument("Port number must contain only digits");
            }
        }

        int port = std::stoi(port_str);
        if (port < 1 || port > 65535) {
            throw std::invalid_argument("Port number must be between 1 and 65535");
        }

        // Store host and port for later use
        this->host = host;
        this->port = port;
        
        // Initialize HTTP client with better timeout settings
        cli = std::make_unique<::httplib::Client>(host.c_str(), port);
        cli->set_connection_timeout(3);  // 3 seconds connection timeout
        cli->set_read_timeout(5);        // 5 seconds read timeout
        cli->set_write_timeout(5);       // 5 seconds write timeout

        Logger::info("ServerThread initialized with host: " + host + " port: " + port_str);
    } catch (const std::exception& e) {
        Logger::error("ServerThread initialization error: " + std::string(e.what()));
        throw; // Re-throw to let the caller handle it
    }
}

void ServerThread::start() {
    if (running) {
        Logger::warning("ServerThread is already running");
        return;
    }

    running = true;
    thread = std::thread(&ServerThread::threadFunction, this);
    Logger::info("ServerThread started successfully");
}

void ServerThread::stop() {
    if (!running) {
        Logger::warning("ServerThread is not running");
        return;
    }

    running = false;
    if (thread.joinable()) {
        thread.join();
    }
    Logger::info("ServerThread stopped");
}

void ServerThread::threadFunction() {
    const int RETRY_INTERVAL_MS = 1000; // 1 second between retries
    const int MAX_RETRIES = 5;          // Max retry attempts
    int retryCount = 0;

    while (running) {
        try {
            json data = prepareData();
            Logger::info("Prepared data: " + data.dump());

            // Enhanced send with retry logic
            bool success = sendWithRetry(data, MAX_RETRIES, RETRY_INTERVAL_MS);
            
            if (success) {
                retryCount = 0; // Reset retry counter on success
                connectionStatus = true;
            } else {
                retryCount++;
                connectionStatus = false;
                Logger::warning("Failed to send data after " + std::to_string(retryCount) + " attempts");
            }

            // Sleep between sends (adjustable based on needs)
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        } catch (const std::exception& e) {
            Logger::error("Exception in threadFunction: " + std::string(e.what()));
            std::this_thread::sleep_for(std::chrono::milliseconds(RETRY_INTERVAL_MS));
        }
    }
}

bool ServerThread::sendWithRetry(const json& data, int maxRetries, int retryIntervalMs) {
    for (int attempt = 1; attempt <= maxRetries; ++attempt) {
        try {
            if (!cli->is_socket_open()) {
                Logger::info("Attempting to reconnect (attempt " + std::to_string(attempt) + ")");
                cli = std::make_unique<::httplib::Client>(host.c_str(), port);
                cli->set_connection_timeout(3);
                cli->set_read_timeout(5);
            }

            auto res = cli->Post("/api/data", data.dump(), "application/json");
            
            if (res) {
                if (res->status == 200) {
                    Logger::info("Data sent successfully");
                    try {
                        // Validate response body is not empty
                        if (res->body.empty()) {
                            Logger::warning("Empty response body from server");
                            return false;
                        }
                        
                        // Log first 100 characters of response for debugging
                        std::string debugPreview = res->body.substr(0, 100);
                        Logger::info("Server response preview: " + debugPreview + (res->body.length() > 100 ? "..." : ""));
                        
                        json response = json::parse(res->body);
                        // Process successful response if needed
                        return true;
                    } catch (const json::parse_error& e) {
                        Logger::error("JSON parse error at position " + std::to_string(e.byte) + ": " + std::string(e.what()));
                        Logger::error("Response body: " + res->body.substr(0, 200));
                    }
                } else {
                    Logger::warning("Server returned HTTP error: " + std::to_string(res->status) + " - " + res->body);
                }
            } else {
                Logger::warning("Network error: " + httplib::to_string(res.error()));
            }
        } catch (const std::exception& e) {
            Logger::error("Error sending data: " + std::string(e.what()));
        }

        if (attempt < maxRetries) {
            std::this_thread::sleep_for(std::chrono::milliseconds(retryIntervalMs));
        }
    }
    return false;
}

json ServerThread::prepareData() {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);
    
    json data;
    data["speed"] = vehicleData->speed;
    data["mode"] = vehicleData->mode;
    data["battery"] = vehicleData->battery;
    data["battery_temp"] = vehicleData->battery_temp;
    data["gear"] = vehicleData->gear;
    data["estimated_distance"] = vehicleData->estimated_distance;
    data["distance_traveled"] = vehicleData->distance_traveled;
    data["signal_left"] = vehicleData->signal_left;
    data["signal_right"] = vehicleData->signal_right;
    data["wind"] = vehicleData->wind;
    data["air_condition"] = vehicleData->air_condition;
    data["plug_in"] = vehicleData->plug_in;
    data["door_lock"] = vehicleData->door_lock;
    data["seat_belt"] = vehicleData->seat_belt;
    data["park"] = vehicleData->park;
    data["brake"] = vehicleData->brake;
    data["gas"] = vehicleData->gas;
    data["warning"] = vehicleData->warning;
    data["temperature"] = vehicleData->temperature;
    data["altitude"] = vehicleData->altitude;
    data["weather"] = vehicleData->weather;
    data["engine_power"] = vehicleData->engine_power;
    data["engine_torque"] = vehicleData->engine_torque;
    data["engine_temp"] = vehicleData->engine_temp;
    data["brake_pressure"] = vehicleData->brake_pressure;
    data["abs_active"] = vehicleData->abs_active;
    data["esp_active"] = vehicleData->esp_active;
    
    // Add timestamp
    auto now = std::chrono::system_clock::now();
    data["timestamp"] = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()).count();
    
    return data;
}

bool ServerThread::isRunning() const {
    return running;
}

bool ServerThread::isConnected() const {
    return connectionStatus;
}

ServerThread::~ServerThread() {
    stop();
#ifdef _WIN32
    WSACleanup();
    Logger::info("Winsock cleanup completed");
#endif
}