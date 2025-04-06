#ifndef ENVIRONMENTAL_CONDITION_H
#define ENVIRONMENTAL_CONDITION_H

#include <string>

/**
 * @enum WeatherType
 * @brief Represents different weather conditions
 */
enum class WeatherType {
    CLEAR,
    RAIN,
    SNOW,
    FOG
};

/**
 * @class EnvironmentalCondition
 * @brief Represents environmental conditions affecting vehicle performance
 */
class EnvironmentalCondition {
public:
    std::string getWeatherCondition() const;

private:
    double temperature;     // Celsius
    double humidity;        // percentage (0-100)
    double altitude;        // meters
    double slope;           // percentage (0-100)
    double roughness;       // road roughness factor (0-1)
    double load;            // additional load in kg
    double windSpeed;       // meters per second
    WeatherType weather;
    
public:
    /**
     * @brief Default constructor
     */
    EnvironmentalCondition();
    
    /**
     * @brief Parameterized constructor
     * @param temp Temperature in Celsius
     * @param alt Altitude in meters
     * @param weather Weather condition
     */
    EnvironmentalCondition(double temp, double alt, WeatherType weather);
    
    /**
     * @brief Destructor
     */
    ~EnvironmentalCondition();
    
    // Getters
    double getTemperature() const;
    double getHumidity() const;
    double getAltitude() const;
    double getSlope() const;
    double getRoughness() const;
    double getLoad() const;
    double getWindSpeed() const;
    WeatherType getWeather() const;
    
    // Setters
    void setTemperature(double temp);
    void setHumidity(double humidity);
    void setAltitude(double alt);
    void setSlope(double slope);
    void setRoughness(double roughness);
    void setLoad(double load);
    void setWindSpeed(double speed);
    void setWeather(WeatherType weather);
    
    /**
     * @brief Calculate impact on engine power
     * @return Power impact factor (0-1)
     */
    double calculatePowerImpact() const;
    
    /**
     * @brief Calculate impact on vehicle range
     * @return Range impact factor (0-1)
     */
    double calculateRangeImpact() const;
    
    /**
     * @brief Calculate impact on maximum speed
     * @return Max speed impact factor (0-1)
     */
    double calculateMaxSpeedImpact() const;
    
    /**
     * @brief Calculate tire friction based on weather and road conditions
     * @return Friction coefficient (0-1)
     */
    double calculateTireFriction() const;
    
    /**
     * @brief Calculate visibility based on weather conditions
     * @return Visibility factor (0-1)
     */
    double calculateVisibility() const;
    
    /**
     * @brief Get weather as string
     * @return Weather string
     */
    std::string getWeatherString() const;
    
    /**
     * @brief Get environment status as string
     * @return Status string
     */
    std::string getStatusString() const;

    /**
     * @brief Calculate ambient temperature based on environmental factors
     * @return Calculated ambient temperature in Celsius
     */
    double calculateAmbientTemperature() const;



    /**
     * @brief Calculate battery temperature according to SAE J2931 standard
     * @param temperature Current ambient temperature
     * @param load Current battery load
     * @return Calculated battery temperature in Celsius
     */
    double calculateBatteryTempSAEJ2931(double temperature, double load) const;
    

    /**
     * @brief Generate random weather values based on current weather type
     */
    void generateRandomWeatherValues();
};

#endif // ENVIRONMENTAL_CONDITION_H
