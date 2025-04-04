#ifndef SENSOR_H
#define SENSOR_H

#include <string>

/**
 * @enum SensorType
 * @brief Represents different types of sensors in the vehicle
 */
enum class SensorType {
    SPEED,
    TEMPERATURE,
    VOLTAGE,
    CURRENT,
    PROXIMITY,
    ACCELERATION,
    ALTITUDE
};

/**
 * @class Sensor
 * @brief Represents a sensor in the vehicle
 */
class Sensor {
private:
    SensorType type;
    double value;
    double accuracy;
    bool connected;
    double updateFrequency;  // Hz
    
public:
    /**
     * @brief Constructor with sensor type
     * @param type Type of sensor
     */
    Sensor(SensorType type);
    
    /**
     * @brief Constructor with sensor type and update frequency
     * @param type Type of sensor
     * @param updateFreq Update frequency in Hz
     */
    Sensor(SensorType type, double updateFreq);
    
    /**
     * @brief Destructor
     */
    ~Sensor();
    
    // Getters
    SensorType getType() const;
    double getValue() const;
    double getAccuracy() const;
    bool isConnected() const;
    double getUpdateFrequency() const;
    
    // Setters
    void setValue(double value);
    void setAccuracy(double accuracy);
    void setConnected(bool state);
    void setUpdateFrequency(double freq);
    
    /**
     * @brief Read sensor value
     * @return Current sensor value with noise if connected, -1 otherwise
     */
    double read() const;
    
    /**
     * @brief Read sensor value (alias for read())
     * @return Current sensor value with noise if connected, -1 otherwise
     */
    double readValue() const { return read(); }
    
    /**
     * @brief Update sensor with new real value
     * @param realValue Real value to update sensor with
     * @return Sensor reading after update
     */
    double update(double realValue);
    
    /**
     * @brief Disconnect sensor (simulate fault)
     */
    void disconnect();
    
    /**
     * @brief Reconnect sensor
     */
    void reconnect();
    
    /**
     * @brief Add noise to sensor reading
     * @param value Real value
     * @return Value with noise added
     */
    double addNoise(double value) const;
    
    /**
     * @brief Get sensor type as string
     * @param type Sensor type
     * @return Sensor type string
     */
    static std::string getTypeString(SensorType type);
    
    /**
     * @brief Get current sensor type as string
     * @return Current sensor type string
     */
    std::string getTypeString() const;
    
    /**
     * @brief Get sensor status as string
     * @return Status string
     */
    std::string getStatusString() const;
};

#endif // SENSOR_H