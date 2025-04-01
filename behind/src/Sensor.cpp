#include "../include/Sensor.h"
#include <cmath>
#include <random>
#include <sstream>
#include <iomanip>

// Random number generator for sensor noise
static std::random_device rd;
static std::mt19937 gen(rd());

Sensor::Sensor(SensorType type)
    : type(type),
      value(0.0),
      accuracy(0.98),  // 98% accuracy by default
      connected(true),
      updateFrequency(100.0) {  // 100 Hz as per Database.md
}

Sensor::Sensor(SensorType type, double updateFreq)
    : type(type),
      value(0.0),
      accuracy(0.98),
      connected(true),
      updateFrequency(updateFreq) {
}

Sensor::~Sensor() {
    // Cleanup if needed
}

// Getters
SensorType Sensor::getType() const {
    return type;
}

double Sensor::getValue() const {
    return value;
}

double Sensor::getAccuracy() const {
    return accuracy;
}

bool Sensor::isConnected() const {
    return connected;
}

double Sensor::getUpdateFrequency() const {
    return updateFrequency;
}

// Setters
void Sensor::setValue(double value) {
    this->value = value;
}

void Sensor::setAccuracy(double accuracy) {
    // Ensure accuracy is between 0 and 1
    this->accuracy = std::max(0.0, std::min(1.0, accuracy));
}

void Sensor::setConnected(bool state) {
    this->connected = state;
}

void Sensor::setUpdateFrequency(double freq) {
    // Ensure frequency is positive
    this->updateFrequency = std::max(1.0, freq);
}

double Sensor::read() const {
    if (!connected) {
        return -1.0;  // Indicate disconnected sensor
    }
    
    // Return current value with noise
    return value;
}

double Sensor::update(double realValue) {
    if (!connected) {
        return -1.0;  // Indicate disconnected sensor
    }
    
    // Update value with noise
    value = addNoise(realValue);
    
    return value;
}

void Sensor::disconnect() {
    connected = false;
}

void Sensor::reconnect() {
    connected = true;
}

double Sensor::addNoise(double value) const {
    if (!connected) {
        return -1.0;
    }
    
    // Create a normal distribution based on accuracy
    // Higher accuracy means less noise
    double stddev = (1.0 - accuracy) * std::abs(value) * 0.1;
    std::normal_distribution<double> noise(0.0, stddev);
    
    // Add noise to value
    double noisyValue = value + noise(gen);
    
    // Different sensor types might have different noise characteristics
    switch (type) {
        case SensorType::SPEED:
            // Speed sensors are more accurate at higher speeds
            if (value < 5.0) {
                noisyValue = value + noise(gen) * 2.0;
            }
            break;
            
        case SensorType::TEMPERATURE:
            // Temperature sensors have more noise at extreme temperatures
            if (value < -10.0 || value > 80.0) {
                noisyValue = value + noise(gen) * 1.5;
            }
            break;
            
        case SensorType::PROXIMITY:
            // Proximity sensors have more noise at longer distances
            if (value > 50.0) {
                noisyValue = value + noise(gen) * 1.8;
            }
            break;
            
        default:
            // Default noise model already applied
            break;
    }
    
    return noisyValue;
}

std::string Sensor::getTypeString(SensorType type) {
    switch (type) {
        case SensorType::SPEED:
            return "Speed";
        case SensorType::TEMPERATURE:
            return "Temperature";
        case SensorType::VOLTAGE:
            return "Voltage";
        case SensorType::CURRENT:
            return "Current";
        case SensorType::PROXIMITY:
            return "Proximity";
        case SensorType::ACCELERATION:
            return "Acceleration";
        default:
            return "Unknown";
    }
}

std::string Sensor::getTypeString() const {
    return getTypeString(type);
}

std::string Sensor::getStatusString() const {
    std::stringstream ss;
    ss << "Sensor Status (" << getTypeString() << "):" << std::endl;
    ss << "  Connected: " << (connected ? "Yes" : "No") << std::endl;
    ss << "  Current Value: " << std::fixed << std::setprecision(2) << value << std::endl;
    ss << "  Accuracy: " << std::fixed << std::setprecision(1) << (accuracy * 100.0) << "%" << std::endl;
    ss << "  Update Frequency: " << std::fixed << std::setprecision(1) << updateFrequency << " Hz" << std::endl;
    
    return ss.str();
}
