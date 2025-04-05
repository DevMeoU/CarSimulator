#include "ThreadHandlers.h"
#include <Windows.h>
#include <iostream>

void KeyboardHandler::threadFunction() {
    std::cout << "[Keyboard] Thread started" << std::endl;
    
    static std::unordered_map<int, bool> keyStates;
    static std::unordered_map<int, bool> toggleStates;
    static std::unordered_map<int, std::chrono::steady_clock::time_point> keyPressTimes;
    static std::chrono::steady_clock::time_point lastUpdate;
    static std::chrono::steady_clock::time_point lastVehicleUpdate;
    static bool initialized = false;
    if (!initialized) {
        lastUpdate = std::chrono::steady_clock::now();
        lastVehicleUpdate = std::chrono::steady_clock::now();
        initialized = true;
    }

    while (running) {
        try {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate);
            auto vehicleElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastVehicleUpdate);
            
            if (elapsed.count() >= 16) { // ~60 key checks per second
                lastUpdate = now;
                
                std::function<void(int, std::function<void(bool)>, const char*, bool)> checkKey = 
                    [&](int key, std::function<void(bool)> action, const char* actionName, bool isToggle = false) {
                        bool currentState = (GetAsyncKeyState(key) & 0x8000) != 0;
                        bool wasPressed = keyStates[key];
                        
                        if (currentState) {
                            if (!wasPressed) {
                                // Key just pressed
                                keyPressTimes[key] = std::chrono::steady_clock::now();
                                if (isToggle) {
                                    toggleStates[key] = !toggleStates[key];
                                    std::cout << "[Keyboard] Key pressed: " << (key < 32 ? std::to_string(key) : std::string(1, static_cast<char>(key)))
                                              << " (" << actionName << ": " << (toggleStates[key] ? "ON" : "OFF") << ")" << std::endl;
                                    action(toggleStates[key]);
                                } else {
                                    std::cout << "[Keyboard] Key pressed: " << (key < 32 ? std::to_string(key) : std::string(1, static_cast<char>(key)))
                                              << " (" << actionName << ")" << std::endl;
                                    action(true);
                                }
                            } else {
                                // Key being held
                                if (!isToggle) {
                                    action(true);
                                }
                            }
                        } else if (wasPressed) {
                            // Key just released
                            if (!isToggle) {
                                action(false);
                            }
                        }
                        
                        keyStates[key] = currentState;
                    };

                // Phím ESC: Thoát khỏi chương trình
                checkKey(VK_ESCAPE, [&](bool) { running = false; }, "exit", false);
                // Phím SPACE: Tăng tốc xe với gia tốc 0.5
                checkKey(VK_SPACE, [&](bool success) {
                    if (success) {
                        auto now = std::chrono::steady_clock::now();
                        auto pressDuration = std::chrono::duration_cast<std::chrono::milliseconds>(now - keyPressTimes[VK_SPACE]);
                        // Thời gian phản hồi 100-300ms
                        if (pressDuration.count() >= 100) {
                            if (vehicleData->seat_belt || vehicleData->speed < 20) {
                                // Gia tốc 2.78 m/s² để đạt 100km/h trong 10s
                                vehicle.accelerate(2.78);
                                std::cout << "[Keyboard] Accelerating at 2.78 m/s²" << std::endl;
                            } else {
                                vehicleData->warning = "WARNING: Seat belt not fastened at high speed!";
                            }
                        }
                    } else {
                        // Khi nhả ga, áp dụng phanh tái sinh với gia tốc âm -2 m/s²
                        vehicle.decelerate(2.0);
                        std::cout << "[Keyboard] Regenerative braking at -2.0 m/s²" << std::endl;
                    }
                }, "accelerate (space)", false);
                checkKey(VK_RETURN, [&](bool success) { 
                    auto now = std::chrono::steady_clock::now();
                    auto pressDuration = std::chrono::duration_cast<std::chrono::milliseconds>(now - keyPressTimes[VK_RETURN]);
                    
                    if (success) {
                        // Thời gian phản hồi phanh khi nhấn
                        if (pressDuration.count() >= 100) {
                            // Gia tốc phanh -7.5 m/s² (trung bình của -6 đến -9)
                            vehicle.brake(7.5);
                            vehicleData->brake = true;
                            std::cout << "[Keyboard] Braking at -7.5 m/s²" << std::endl;
                            if (vehicleData->speed > 100 && vehicleData->brake_pressure < 50) {
                                vehicleData->warning = "WARNING: Low brake pressure at high speed!";
                            }
                        }
                    } else if (pressDuration.count() >= 100) {
                        // Thời gian phản hồi khi nhả phanh
                        vehicle.brake(0);
                        vehicleData->brake = false;
                        std::cout << "[Keyboard] Brake released" << std::endl;
                    }
                }, "brake (enter)", false);
                checkKey('D', [&](bool success) { if (success) vehicle.setGear("P"); }, "park gear", false);
                checkKey('D', [&](bool success) { if (success) vehicle.setGear("D"); }, "drive gear", false);
                // Phím R: Chuyển sang chế độ lùi (Reverse)
                checkKey('R', [&](bool success) { 
                    if (success && vehicleData->speed <= 10) {
                        vehicle.setGear("R"); 
                    } else if (success) {
                        vehicleData->warning = "WARNING: Cannot shift to reverse at high speed!";
                    }
                }, "reverse gear", false);
                checkKey('N', [&](bool success) { if (success) vehicle.setGear("N"); }, "neutral gear", false);
                checkKey('P', [&](bool state) { 
                    if (vehicleData->speed == 0 || state) {
                        vehicle.setParking(state); 
                    } else {
                        vehicleData->warning = "WARNING: Cannot release parking brake while moving!";
                    }
                }, "parking", true);
                checkKey('V', [&](bool state) { vehicle.setLeftSignalOn(state); }, "left signal", true);
                checkKey('B', [&](bool state) { vehicle.setRightSignalOn(state); }, "right signal", true);
                checkKey('L', [&](bool state) { vehicle.setDoorLocked(state); }, "door lock", true);
                checkKey('S', [&](bool state) { vehicle.setSeatbeltOn(state); }, "seat belt", true);
                checkKey('A', [&](bool state) { 
                    try {
                        vehicle.getEnvironment().setAirConditioningLevel(state ? 1 : 0);
                    } catch (const std::exception& e) {
                        std::cerr << "[Keyboard] Error setting air conditioning: " << e.what() << std::endl;
                    }
                }, "air conditioning", true);
                checkKey('H', [&](bool state) { vehicle.setLeftSignalOn(state);
                vehicle.setRightSignalOn(state); }, "hazard lights", true);
                
                checkKey('1', [&](bool) { vehicle.changeDrivingMode(DrivingModeType::NORMAL); }, "normal mode", false);
                checkKey('2', [&](bool) { vehicle.changeDrivingMode(DrivingModeType::SPORT); }, "sport mode", false);
                checkKey('3', [&](bool) { vehicle.changeDrivingMode(DrivingModeType::ECO); }, "eco mode", false);
                checkKey('4', [&](bool) { vehicle.changeDrivingMode(DrivingModeType::SNOW_OFFROAD); }, "snow/offroad mode", false);
            }
            
            // Update vehicle state at a fixed interval
            if (vehicleElapsed.count() >= 16) { // ~60 updates per second
                lastVehicleUpdate = now;
                double deltaTime = vehicleElapsed.count() / 1000.0;
                
                try {
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
                    std::cout << "[Keyboard] Gas: " << (vehicleData->gas? "ON" : "OFF") << std::endl;
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

                    /* Update vehicle state */
                    vehicle.update(deltaTime);
                } catch (const std::exception& e) {
                    std::cerr << "[Keyboard] Error updating vehicle state: " << e.what() << std::endl;
                }
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Reduce sleep time for faster key response
        } catch (const std::exception& e) {
            std::cerr << "[Keyboard] Critical error in thread loop: " << e.what() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Prevent tight error loop
        }
    }
    
    std::cout << "[Keyboard] Thread stopped" << std::endl;
}