#include "../include/EnvironmentalCondition.h"
#include <cmath>
#include <sstream>
#include <iomanip>

EnvironmentalCondition::EnvironmentalCondition()
    : temperature(20.0),    // Normal temperature
      altitude(0.0),        // Sea level
      slope(0.0),           // Flat road
      roughness(0.0),       // Smooth road
      load(0.0),            // No additional load
      windSpeed(0.0),       // No wind
      weather(WeatherType::CLEAR) {
}

EnvironmentalCondition::EnvironmentalCondition(double temp, double alt, WeatherType weather)
    : temperature(temp),
      altitude(alt),
      slope(0.0),
      roughness(0.0),
      load(0.0),
      windSpeed(0.0),
      weather(weather) {
}

EnvironmentalCondition::~EnvironmentalCondition() {
    // Cleanup if needed
}

// Getters
double EnvironmentalCondition::getTemperature() const {
    return temperature;
}

double EnvironmentalCondition::getAltitude() const {
    return altitude;
}

double EnvironmentalCondition::getSlope() const {
    return slope;
}

double EnvironmentalCondition::getRoughness() const {
    return roughness;
}

double EnvironmentalCondition::getLoad() const {
    return load;
}

double EnvironmentalCondition::getWindSpeed() const {
    return windSpeed;
}

WeatherType EnvironmentalCondition::getWeather() const {
    return weather;
}

// Setters
void EnvironmentalCondition::setTemperature(double temp) {
    this->temperature = temp;
}

void EnvironmentalCondition::setAltitude(double alt) {
    this->altitude = alt;
}

void EnvironmentalCondition::setSlope(double slope) {
    // Ensure slope is between 0 and 100
    this->slope = std::max(0.0, std::min(100.0, slope));
}

void EnvironmentalCondition::setRoughness(double roughness) {
    // Ensure roughness is between 0 and 1
    this->roughness = std::max(0.0, std::min(1.0, roughness));
}

void EnvironmentalCondition::setLoad(double load) {
    // Ensure load is non-negative
    this->load = std::max(0.0, load);
}

void EnvironmentalCondition::setWindSpeed(double speed) {
    // Ensure wind speed is non-negative
    this->windSpeed = std::max(0.0, speed);
}

void EnvironmentalCondition::setWeather(WeatherType weather) {
    this->weather = weather;
}

double EnvironmentalCondition::calculatePowerImpact() const {
    double powerFactor = 1.0;
    
    // Temperature impact as per Database.md
    if (temperature < 0) {
        // Cold temperature reduces battery efficiency but not directly engine power
        powerFactor *= 0.95;
    } else if (temperature > 35) {
        // Above 35°C: Reduce engine performance by 10%
        powerFactor *= 0.9;
    }
    
    // Altitude impact as per Database.md
    // 5% reduction per 1000m
    powerFactor *= (1.0 - (altitude / 1000.0) * 0.05);
    
    // Ensure factor is within reasonable bounds
    return std::max(0.5, std::min(1.0, powerFactor));
}

double EnvironmentalCondition::calculateRangeImpact() const {
    double rangeFactor = 1.0;
    
    // Temperature impact
    if (temperature < 0) {
        // Below 0°C: Reduce range by 20-30%
        rangeFactor *= 0.75;
    } else if (temperature > 35) {
        // Above 35°C: Reduce range slightly
        rangeFactor *= 0.95;
    }
    
    // Altitude has minimal direct impact on range
    
    // Slope impact
    if (slope > 0) {
        // 25% increase in energy consumption for 10% slope
        rangeFactor *= (1.0 - (slope / 10.0) * 0.25);
    }
    
    // Roughness impact
    if (roughness > 0.5) {
        // Rough terrain reduces range
        rangeFactor *= (1.0 - roughness * 0.2);
    }
    
    // Load impact as per Database.md
    // 5-8% reduction per 100kg
    rangeFactor *= (1.0 - (load / 100.0) * 0.065);
    
    // Weather impact
    switch (weather) {
        case WeatherType::RAIN:
            rangeFactor *= 0.95;  // Rain slightly reduces range
            break;
        case WeatherType::SNOW:
            rangeFactor *= 0.85;  // Snow significantly reduces range
            break;
        case WeatherType::FOG:
            rangeFactor *= 0.97;  // Fog slightly reduces range
            break;
        case WeatherType::CLEAR:
        default:
            break;
    }
    
    // Ensure factor is within reasonable bounds
    return std::max(0.5, std::min(1.0, rangeFactor));
}

double EnvironmentalCondition::calculateMaxSpeedImpact() const {
    double speedFactor = 1.0;
    
    // Temperature has minimal direct impact on max speed
    
    // Altitude impact is minimal on max speed
    
    // Slope impact
    if (slope > 5) {
        // Steep slopes reduce max speed
        speedFactor *= (1.0 - (slope - 5) / 95.0 * 0.5);
    }
    
    // Roughness impact as per Database.md
    // Rough roads reduce max speed by up to 20%
    speedFactor *= (1.0 - roughness * 0.2);
    
    // Load has minimal direct impact on max speed
    
    // Weather impact
    switch (weather) {
        case WeatherType::RAIN:
            speedFactor *= 0.9;  // Rain reduces safe max speed
            break;
        case WeatherType::SNOW:
            speedFactor *= 0.7;  // Snow significantly reduces safe max speed
            break;
        case WeatherType::FOG:
            speedFactor *= 0.8;  // Fog reduces safe max speed due to visibility
            break;
        case WeatherType::CLEAR:
        default:
            break;
    }
    
    // Ensure factor is within reasonable bounds
    return std::max(0.5, std::min(1.0, speedFactor));
}

double EnvironmentalCondition::calculateTireFriction() const {
    double frictionCoefficient = 1.0;  // Base friction on dry, smooth road
    
    // Weather impact on friction
    switch (weather) {
        case WeatherType::RAIN:
            // Rain reduces friction by 30% as per Database.md
            frictionCoefficient *= 0.7;
            break;
        case WeatherType::SNOW:
            // Snow reduces friction by 50% as per Database.md
            frictionCoefficient *= 0.5;
            break;
        case WeatherType::FOG:
            // Fog may make road damp
            frictionCoefficient *= 0.9;
            break;
        case WeatherType::CLEAR:
        default:
            break;
    }
    
    // Roughness impact on friction
    // Rougher roads can have better grip up to a point, then worse
    if (roughness < 0.3) {
        frictionCoefficient *= (1.0 + roughness * 0.1);
    } else {
        frictionCoefficient *= (1.03 - (roughness - 0.3) * 0.2);
    }
    
    // Temperature impact on friction
    if (temperature < 5) {
        // Cold temperatures can reduce tire grip
        frictionCoefficient *= (0.9 + temperature / 50.0);
    } else if (temperature > 40) {
        // Very hot temperatures can affect tire performance
        frictionCoefficient *= (1.0 - (temperature - 40) / 100.0);
    }
    
    // Ensure coefficient is within reasonable bounds
    return std::max(0.3, std::min(1.1, frictionCoefficient));
}

double EnvironmentalCondition::calculateVisibility() const {
    double visibilityFactor = 1.0;  // Perfect visibility
    
    // Weather impact on visibility
    switch (weather) {
        case WeatherType::RAIN:
            visibilityFactor = 0.7;  // Rain reduces visibility
            break;
        case WeatherType::SNOW:
            visibilityFactor = 0.5;  // Snow significantly reduces visibility
            break;
        case WeatherType::FOG:
            // Fog reduces visibility to less than 50m as per Database.md
            visibilityFactor = 0.3;
            break;
        case WeatherType::CLEAR:
        default:
            break;
    }
    
    // Altitude can affect visibility (thin air, clouds)
    if (altitude > 2000) {
        visibilityFactor *= (1.0 - (altitude - 2000) / 8000.0 * 0.3);
    }
    
    // Ensure factor is within reasonable bounds
    return std::max(0.1, std::min(1.0, visibilityFactor));
}

std::string EnvironmentalCondition::getWeatherString() const {
    switch (weather) {
        case WeatherType::RAIN:
            return "Rain";
        case WeatherType::SNOW:
            return "Snow";
        case WeatherType::FOG:
            return "Fog";
        case WeatherType::CLEAR:
        default:
            return "Clear";
    }
}

std::string EnvironmentalCondition::getStatusString() const {
    std::stringstream ss;
    ss << "Environmental Conditions:" << std::endl;
    ss << "  Temperature: " << std::fixed << std::setprecision(1) << temperature << "°C" << std::endl;
    ss << "  Altitude: " << std::fixed << std::setprecision(0) << altitude << " m" << std::endl;
    ss << "  Road Slope: " << std::fixed << std::setprecision(1) << slope << "%" << std::endl;
    ss << "  Road Roughness: " << std::fixed << std::setprecision(2) << roughness << " (0-1)" << std::endl;
    ss << "  Additional Load: " << std::fixed << std::setprecision(0) << load << " kg" << std::endl;
    ss << "  Weather: " << getWeatherString() << std::endl;
    ss << "  Tire Friction: " << std::fixed << std::setprecision(2) << calculateTireFriction() << std::endl;
    ss << "  Visibility: " << std::fixed << std::setprecision(2) << calculateVisibility() << std::endl;
    
    return ss.str();
}
