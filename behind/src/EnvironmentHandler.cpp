#include "ThreadHandlers.h"
#include <iostream>

void EnvironmentHandler::threadFunction() {
    std::cout << "[Environment] Thread started" << std::endl;
    
    while (running) {
        std::cout << "[Environment] Reading sensor data..." << std::endl;
        EnvironmentalCondition env;
        Sensor tempSensor(SensorType::TEMPERATURE);
        Sensor altSensor(SensorType::ALTITUDE);

        double temperature = tempSensor.readValue();
        double altitude = altSensor.readValue();

        std::cout << "[Environment] Updating environment - Temperature: " << temperature << "°C, Altitude: " << altitude << "m" << std::endl;
        
        vehicle.updateEnvironment(
            env.calculateAmbientTemperature(),
            env.getHumidity(),
            env.getWindSpeed(),
            altitude
        );

        vehicle.getBattery().updateTemperature(
            env.calculateBatteryTempSAEJ2931(temperature, vehicle.getBatteryLoad()),
            std::chrono::system_clock::now()
        );

        vehicle.getEngine().adjustPerformanceForAltitude(altitude);

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    
    std::cout << "[Environment] Thread stopped" << std::endl;
}