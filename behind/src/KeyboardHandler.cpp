#include "ThreadHandlers.h"
#include <Windows.h>
#include <iostream>

void KeyboardHandler::threadFunction() {
    std::cout << "[Keyboard] Thread started" << std::endl;
    
    static std::unordered_map<int, bool> keyStates;
    static std::unordered_map<int, bool> toggleStates;
    static auto lastUpdate = std::chrono::steady_clock::now();
    static auto lastVehicleUpdate = std::chrono::steady_clock::now();

    while (running) {
        try {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate);
            auto vehicleElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastVehicleUpdate);
            
            if (elapsed.count() >= 50) {
                lastUpdate = now;
                
                auto checkKey = [&](int key, auto action, const char* actionName, bool isToggle = false) {
                    bool currentState = (GetAsyncKeyState(key) & 0x8000) != 0;
                    if (currentState && !keyStates[key]) {
                        try {
                            if (isToggle) {
                                toggleStates[key] = !toggleStates[key];
                                std::cout << "[Keyboard] Key pressed: " << (key < 32 ? std::to_string(key) : std::string(1, static_cast<char>(key)))
                                          << " (" << actionName << ": " << (toggleStates[key] ? "ON" : "OFF") << ")" << std::endl;
                            } else {
                                std::cout << "[Keyboard] Key pressed: " << (key < 32 ? std::to_string(key) : std::string(1, static_cast<char>(key)))
                                          << " (" << actionName << ")" << std::endl;
                            }
                            action(isToggle ? toggleStates[key] : true);
                        } catch (const std::exception& e) {
                            std::cerr << "[Keyboard] Error executing action '" << actionName << "': " << e.what() << std::endl;
                        }
                    }
                    keyStates[key] = currentState;
                };
                
                // Phím SPACE: Tăng tốc xe với gia tốc 0.5
                checkKey(VK_SPACE, [&](bool) { vehicle.accelerate(0.5); }, "accelerate");
                // Phím ENTER: Phanh xe với lực phanh 2
                checkKey(VK_RETURN, [&](bool) { vehicle.brake(2); }, "brake");
                // Phím P: Chuyển sang chế độ lái (Park)
                checkKey('D', [&](bool) { vehicle.setGear("P"); }, "park gear");
                // Phím D: Chuyển sang chế độ lái (Drive)
                checkKey('D', [&](bool) { vehicle.setGear("D"); }, "drive gear");
                // Phím R: Chuyển sang chế độ lùi (Reverse)
                checkKey('R', [&](bool) { vehicle.setGear("R"); }, "reverse gear");
                // Phím N: Chuyển sang chế độ trung gian (Neutral)
                checkKey('N', [&](bool) { vehicle.setGear("N"); }, "neutral gear");
                // Phím P: Bật/tắt phanh tay (toggle)
                checkKey('P', [&](bool state) { vehicle.setParking(state); }, "parking", true);
                // Phím V: Bật/tắt đèn xi nhan trái (toggle)
                checkKey('V', [&](bool state) { vehicle.setLeftSignalOn(state); }, "left signal", true);
                // Phím B: Bật/tắt đèn xi nhan phải (toggle)
                checkKey('B', [&](bool state) { vehicle.setRightSignalOn(state); }, "right signal", true);
                // Phím L: Khóa/mở khóa cửa xe (toggle)
                checkKey('L', [&](bool state) { vehicle.setDoorLocked(state); }, "door lock", true);
                // Phím S: Bật/tắt dây an toàn (toggle)
                checkKey('S', [&](bool state) { vehicle.setSeatbeltOn(state); }, "seat belt", true);
                // Phím A: Bật/tắt điều hòa không khí (toggle)
                checkKey('A', [&](bool state) { 
                    try {
                        vehicle.getEnvironment().setAirConditioningLevel(state ? 1 : 0);
                    } catch (const std::exception& e) {
                        std::cerr << "[Keyboard] Error setting air conditioning: " << e.what() << std::endl;
                    }
                }, "air conditioning", true);
                // Phím H: Bật/tắt đèn cảnh báo nguy hiểm (toggle)
                checkKey('H', [&](bool state) { vehicle.setLeftSignalOn(state);
                vehicle.setRightSignalOn(state); }, "hazard lights", true);
                
                // Phím 1: Chuyển sang chế độ lái thường (Normal mode)
                checkKey('1', [&](bool) { vehicle.changeDrivingMode(DrivingModeType::NORMAL); }, "normal mode");
                // Phím 2: Chuyển sang chế độ thể thao (Sport mode)
                checkKey('2', [&](bool) { vehicle.changeDrivingMode(DrivingModeType::SPORT); }, "sport mode");
                // Phím 3: Chuyển sang chế độ tiết kiệm nhiên liệu (Eco mode)
                checkKey('3', [&](bool) { vehicle.changeDrivingMode(DrivingModeType::ECO); }, "eco mode");
                // Phím 4: Chuyển sang chế độ tuyết/địa hình (Snow/Offroad mode)
                checkKey('4', [&](bool) { vehicle.changeDrivingMode(DrivingModeType::SNOW_OFFROAD); }, "snow/offroad mode");
            }
            
            // Update vehicle state at a fixed interval
            if (vehicleElapsed.count() >= 16) { // ~60 updates per second
                lastVehicleUpdate = now;
                double deltaTime = vehicleElapsed.count() / 1000.0;
                
                try {
                    vehicle.update(deltaTime);
                    // Update vehicle data with latest state
                    std::lock_guard<std::mutex> lock(vehicleData->mutex);
                    // Update core vehicle parameters with thread-safe calculations
                    vehicleData->speed = vehicle.getSpeed();
                    vehicleData->gear = vehicleData->gear;
                    vehicleData->mode = vehicleData->mode;
                    
                    // Update battery and energy consumption data
                    vehicleData->battery = vehicle.getBattery().getChargeLevel();
                    vehicleData->battery_temp = vehicle.getBattery().getTemperature();
                    vehicleData->estimated_distance = vehicle.getBattery().getRange();
                    vehicleData->distance_traveled = vehicle.getDistanceTraveled();
                    
                    // Update vehicle status indicators
                    vehicleData->signal_left = vehicle.isLeftSignalOn();
                    vehicleData->signal_right = vehicle.isRightSignalOn();
                    vehicleData->park = vehicleData->park;
                    
                    // Update powertrain parameters with optimized calculations
                    vehicleData->engine_power = vehicle.getEngine().getPower();
                    vehicleData->engine_torque = vehicle.getEngine().getMaxTorque() * vehicle.getEngine().getThrottle();
                    vehicleData->engine_temp = vehicle.getEngine().getTemperature();
                    vehicleData->brake_pressure = vehicle.getSafetySystem().getBrakePower();
                    
                    // Update additional vehicle states
                    auto& safetySystem = vehicle.getSafetySystem();
                    auto& environment = vehicle.getEnvironment();
                    
                    vehicleData->abs_active = safetySystem.isAbsActive();
                    vehicleData->esp_active = safetySystem.isEspActive();
                    vehicleData->brake = vehicle.isBrakeActive();
                    vehicleData->gas = vehicle.isAcceleratorActive();
                    vehicleData->door_lock = vehicle.isDoorLocked();
                    vehicleData->plug_in = vehicle.getBattery().isCharging();
                    vehicleData->seat_belt = vehicle.isSeatbeltOn();
                    
                    // Update environmental conditions
                    vehicleData->air_condition = vehicle.getEnvironment().getAirConditioningLevel();
                    vehicleData->altitude = environment.getAltitude();
                    vehicleData->temperature = environment.getTemperature();
                    vehicleData->weather = environment.getWeatherCondition();
                    vehicleData->wind = environment.getWindSpeed();
                    
                    // Update timestamp and warning
                    vehicleData->timestamp_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::system_clock::now().time_since_epoch()).count();
                    vehicleData->warning = vehicle.getStatusString();
                } catch (const std::exception& e) {
                    std::cerr << "[Keyboard] Error updating vehicle state: " << e.what() << std::endl;
                }
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        } catch (const std::exception& e) {
            std::cerr << "[Keyboard] Critical error in thread loop: " << e.what() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Prevent tight error loop
        }
    }
    
    std::cout << "[Keyboard] Thread stopped" << std::endl;
}