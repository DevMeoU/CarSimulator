#include "../include/Utils.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>

namespace Utils {

double kmhToMs(double kmh) {
    return kmh / 3.6;
}

double msToKmh(double ms) {
    return ms * 3.6;
}

double calculatePower(double force, double speed) {
    return force * speed;  // Power (W) = Force (N) * Speed (m/s)
}

double calculateForce(double mass, double acceleration) {
    return mass * acceleration;  // Force (N) = Mass (kg) * Acceleration (m/s²)
}

double calculateEnergy(double power, double hours) {
    return power * hours;  // Energy (kWh) = Power (kW) * Time (h)
}

double calculateRange(double batteryCapacity, double energyConsumption) {
    if (energyConsumption <= 0) {
        return 0.0;  // Avoid division by zero
    }
    return batteryCapacity / energyConsumption;  // Range (km) = Capacity (kWh) / Consumption (kWh/km)
}

double calculateBrakingDistance(double speed, double frictionCoefficient) {
    // Convert speed to m/s
    double speedMs = kmhToMs(speed);
    
    // Braking distance formula: d = v² / (2 * μ * g)
    // where v is speed in m/s, μ is friction coefficient, g is gravity (9.81 m/s²)
    return (speedMs * speedMs) / (2 * frictionCoefficient * 9.81);
}

double calculateTimeToCollision(double distance, double speed) {
    if (speed <= 0) {
        return 999.0;  // Avoid division by zero and indicate no collision
    }
    
    // Convert speed to m/s
    double speedMs = kmhToMs(speed);
    
    // Time to collision = distance / speed
    return distance / speedMs;
}

void sleep(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void runAtInterval(std::function<void()> func, int intervalMs, int durationMs) {
    auto startTime = std::chrono::steady_clock::now();
    auto endTime = startTime + std::chrono::milliseconds(durationMs);
    
    while (std::chrono::steady_clock::now() < endTime) {
        func();
        sleep(intervalMs);
    }
}

std::string formatTime(double seconds) {
    int mins = static_cast<int>(seconds) / 60;
    int secs = static_cast<int>(seconds) % 60;
    
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << mins << ":" 
       << std::setfill('0') << std::setw(2) << secs;
    
    return ss.str();
}

void clearScreen() {
    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif
}

void pauseExecution() {
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void printProgressBar(double progress, int width) {
    // Ensure progress is between 0 and 1
    progress = std::max(0.0, std::min(1.0, progress));
    
    int pos = static_cast<int>(width * progress);
    
    std::cout << "[";
    for (int i = 0; i < width; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << static_cast<int>(progress * 100.0) << "%\r";
    std::cout.flush();
}

std::string getValidatedInput(const std::string& prompt, 
                             std::function<bool(const std::string&)> validator) {
    std::string input;
    bool valid = false;
    
    while (!valid) {
        std::cout << prompt;
        std::getline(std::cin, input);
        
        valid = validator(input);
        if (!valid) {
            std::cout << "Invalid input. Please try again." << std::endl;
        }
    }
    
    return input;
}

std::map<std::string, std::string> parseArgs(int argc, char* argv[]) {
    std::map<std::string, std::string> args;
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        // Check if argument is a flag (starts with - or --)
        if (arg.substr(0, 2) == "--") {
            std::string key = arg.substr(2);
            
            // Check if next argument is a value (not a flag)
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                args[key] = argv[i + 1];
                ++i;  // Skip next argument as it's a value
            } else {
                args[key] = "true";  // Flag without value
            }
        } else if (arg.substr(0, 1) == "-") {
            std::string key = arg.substr(1);
            
            // Check if next argument is a value (not a flag)
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                args[key] = argv[i + 1];
                ++i;  // Skip next argument as it's a value
            } else {
                args[key] = "true";  // Flag without value
            }
        }
    }
    
    return args;
}

} // namespace Utils
