#include "EnvironmentalCondition.h"
#include <stdexcept>
#include <cmath>
#include <string>

EnvironmentalCondition::EnvironmentalCondition() {
    // Khởi tạo giá trị mặc định
    temperature = 20.0;
    humidity = 50.0;
    altitude = 0.0;
    slope = 0.0;
    roughness = 0.1;
    load = 0.0;
    windSpeed = 0.0;
    weather = WeatherType::CLEAR;
    
    // Tạo giá trị ngẫu nhiên dựa trên thời tiết
    generateRandomWeatherValues();
}

EnvironmentalCondition::EnvironmentalCondition(double temp, double alt, WeatherType weather)
    : temperature(temp), humidity(50.0), altitude(alt), slope(0.0),
      roughness(0.1), load(0.0), windSpeed(0.0), weather(weather) {}

EnvironmentalCondition::~EnvironmentalCondition() {}

// Getters
double EnvironmentalCondition::getTemperature() const { return temperature; }
double EnvironmentalCondition::getHumidity() const { return humidity; }
double EnvironmentalCondition::getAltitude() const { return altitude; }
double EnvironmentalCondition::getSlope() const { return slope; }
double EnvironmentalCondition::getRoughness() const { return roughness; }
double EnvironmentalCondition::getLoad() const { return load; }
double EnvironmentalCondition::getWindSpeed() const { return windSpeed; }
WeatherType EnvironmentalCondition::getWeather() const { return weather; }

// Setters
void EnvironmentalCondition::setTemperature(double temp) { temperature = temp; }
void EnvironmentalCondition::setHumidity(double humidity) { this->humidity = humidity; }
void EnvironmentalCondition::setAltitude(double alt) { altitude = alt; }
void EnvironmentalCondition::setSlope(double slope) { this->slope = slope; }
void EnvironmentalCondition::setRoughness(double roughness) { this->roughness = roughness; }
void EnvironmentalCondition::setLoad(double load) { this->load = load; }
void EnvironmentalCondition::setWindSpeed(double speed) { windSpeed = speed; }
void EnvironmentalCondition::setWeather(WeatherType weather) { 
    this->weather = weather;
    generateRandomWeatherValues();
}



std::string EnvironmentalCondition::getWeatherCondition() const {
    switch(weather) {
        case WeatherType::CLEAR: return "Clear";
        case WeatherType::RAIN: return "Rain";
        case WeatherType::SNOW: return "Snow";
        case WeatherType::FOG: return "Fog";
        default: return "Unknown";
    }
}

std::string EnvironmentalCondition::getWeatherString() const {
    switch(weather) {
        case WeatherType::CLEAR: return "Clear skies with good visibility";
        case WeatherType::RAIN: return "Rainy conditions with reduced visibility";
        case WeatherType::SNOW: return "Snowy conditions with slippery roads";
        case WeatherType::FOG: return "Foggy conditions with poor visibility";
        default: return "Unknown weather conditions";
    }
}

std::string EnvironmentalCondition::getStatusString() const {
    return "Temperature: " + std::to_string(temperature) + "°C, " +
           "Humidity: " + std::to_string(humidity) + "%, " +
           "Altitude: " + std::to_string(altitude) + "m, " +
           "Weather: " + getWeatherString();
}

double EnvironmentalCondition::calculatePowerImpact() const {
    // Simplified power impact calculation
    return 1.0 - (0.01 * humidity) - (0.001 * altitude) - (0.05 * roughness);
}

double EnvironmentalCondition::calculateRangeImpact() const {
    // Simplified range impact calculation
    return 1.0 - (0.005 * temperature) - (0.002 * windSpeed) - (0.0005 * load);
}

double EnvironmentalCondition::calculateMaxSpeedImpact() const {
    // Simplified max speed impact calculation
    return 1.0 - (0.01 * slope) - (0.02 * roughness) - (0.001 * load);
}

double EnvironmentalCondition::calculateTireFriction() const {
    // Simplified tire friction calculation based on weather
    switch(weather) {
        case WeatherType::RAIN: return 0.7;
        case WeatherType::SNOW: return 0.4;
        case WeatherType::FOG: return 0.8;
        default: return 1.0; // Clear weather
    }
}

double EnvironmentalCondition::calculateVisibility() const {
    // Simplified visibility calculation based on weather
    switch(weather) {
        case WeatherType::RAIN: return 0.6;
        case WeatherType::SNOW: return 0.5;
        case WeatherType::FOG: return 0.3;
        default: return 1.0; // Clear weather
    }
}

void EnvironmentalCondition::generateRandomWeatherValues() {
    // Tạo giá trị ngẫu nhiên phù hợp với từng loại thời tiết
    switch(weather) {
        case WeatherType::CLEAR:
            temperature = 20.0 + (rand() % 15); // 20-35°C
            humidity = 30 + (rand() % 40); // 30-70%
            windSpeed = 0 + (rand() % 10); // 0-10 m/s
            break;
        case WeatherType::RAIN:
            temperature = 15.0 + (rand() % 10); // 15-25°C
            humidity = 70 + (rand() % 30); // 70-100%
            windSpeed = 5 + (rand() % 15); // 5-20 m/s
            break;
        case WeatherType::SNOW:
            temperature = -10.0 + (rand() % 15); // -10 to 5°C
            humidity = 60 + (rand() % 30); // 60-90%
            windSpeed = 3 + (rand() % 12); // 3-15 m/s
            break;
        case WeatherType::FOG:
            temperature = 5.0 + (rand() % 10); // 5-15°C
            humidity = 80 + (rand() % 20); // 80-100%
            windSpeed = 0 + (rand() % 5); // 0-5 m/s
            break;
    }
}

double EnvironmentalCondition::calculateAmbientTemperature() const {
    // Tính toán nhiệt độ môi trường theo công thức trong depend.md
    return temperature - (0.0065 * altitude) + (0.1 * windSpeed);
}

double EnvironmentalCondition::calculateBatteryTempSAEJ2931(double temperature, double load) const {
    // Simplified SAE J2931 battery temperature calculation
    return temperature + (0.1 * load) + (0.05 * this->temperature);
}
