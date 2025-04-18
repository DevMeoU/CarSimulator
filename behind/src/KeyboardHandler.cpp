#include "ThreadHandlers.h"
#include "WarningMessages.h"
#include <Windows.h>
#include <iostream>

void KeyboardHandler::threadFunction() {
    std::cout << "[Keyboard] Thread started" << std::endl;
    
    static std::unordered_map<int, bool> keyStates;
    static std::unordered_map<int, bool> toggleStates;
    static std::unordered_map<int, std::chrono::steady_clock::time_point> keyPressTimes;
    static std::chrono::steady_clock::time_point lastUpdate;
    static std::chrono::steady_clock::time_point lastVehicleUpdate;
    static double currentFan = 0.0;
    static bool initialized = false;
    if (!initialized) {
        lastUpdate = std::chrono::steady_clock::now();
        lastVehicleUpdate = std::chrono::steady_clock::now();
        currentFan = vehicle.getFan();
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
                            if (vehicleData->gear == "P" || vehicleData->gear == "N") {
                                std::cout << "[Keyboard] Cannot accelerate in " << vehicleData->gear << " gear" << std::endl;
                                return;
                            }
                            if (vehicleData->seat_belt || vehicleData->speed < 20) {
                                // Gia tốc 2.78 m/s² để đạt 100km/h trong 10s
                                vehicle.accelerate(2.78);
                                std::cout << "[Keyboard] Accelerating at 2.78 m/s²" << std::endl;
                            } else {
                                vehicleData->warning = WARNING_SEATBELT_HIGH_SPEED;
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
                                vehicleData->warning = WARNING_LOW_BRAKE_PRESSURE;
                            }
                        }
                    } else if (pressDuration.count() >= 100) {
                        // Thời gian phản hồi khi nhả phanh
                        vehicle.brake(0);
                        vehicleData->brake = false;
                        std::cout << "[Keyboard] Brake released" << std::endl;
                    }
                }, "brake (enter)", false);
                checkKey('P', [&](bool success) { if (success) vehicle.setGear("P"); }, "park gear", false);
                checkKey('D', [&](bool success) { if (success) vehicle.setGear("D"); }, "drive gear", false);
                // Phím R: Chuyển sang chế độ lùi (Reverse)
                checkKey('R', [&](bool success) { 
                    if (success && vehicleData->speed <= 10) {
                        vehicle.setGear("R"); 
                    } else if (success) {
                        vehicleData->warning = WARNING_REVERSE_HIGH_SPEED;
                    }
                }, "reverse gear", false);
                checkKey('N', [&](bool success) { if (success) vehicle.setGear("N"); }, "neutral gear", false);
                checkKey('P', [&](bool state) { 
                    if (vehicleData->speed == 0 || state) {
                        vehicle.setParking(state); 
                    } else {
                        vehicleData->warning = WARNING_PARKING_WHILE_MOVING;
                    }
                }, "parking", true);
                checkKey('V', [&](bool state) { vehicle.setLeftSignalOn(state); }, "left signal", true);
                checkKey('B', [&](bool state) { vehicle.setRightSignalOn(state); }, "right signal", true);
                checkKey('L', [&](bool state) { vehicle.setDoorLocked(state); }, "door lock", true);
                checkKey('S', [&](bool state) { vehicle.setSeatbeltOn(state); }, "seat belt", true);
                checkKey('A', [&](bool state) { 
                    try {
                        // Khi bật điều hòa, đặt giá trị mặc định là 22 độ
                        // Khi tắt điều hòa, đặt giá trị là 0
                        double newValue = state ? 22.0 : 0.0;
                        vehicle.setAirConditioningLevel(newValue);
                        {
                            std::lock_guard<std::mutex> lock(vehicleData->mutex);
                            vehicleData->air_condition = newValue;
                            vehicleData->ac_state = state;
                        }
                        std::cout << "[Keyboard] AC " << (state ? "ON (22°C)" : "OFF") << std::endl;
                    } catch (const std::exception& e) {
                        std::cerr << "[Keyboard] Error setting air conditioning: " << e.what() << std::endl;
                    }
                }, "air conditioning", true);
                checkKey('H', [&](bool state) { vehicle.setLeftSignalOn(state);
                vehicle.setRightSignalOn(state); }, "hazard lights", true);
                
                // Điều khiển quạt gió và điều hòa bằng phím mũi tên
                checkKey(VK_LEFT, [&](bool success) {
                    static bool wasPressed = false;
                    if (success) {
                        wasPressed = true;
                    } else if (wasPressed) {
                        wasPressed = false;
                        if (currentFan > 0) {
                            currentFan = std::max(0.0, currentFan - 1.0);
                            vehicle.setFan(currentFan);
                            vehicleData->fan = currentFan;
                            std::cout << "[Keyboard] Fan speed decreased to " << currentFan << std::endl;
                        }
                    }
                }, "decrease fan", false);
                
                checkKey(VK_RIGHT, [&](bool success) {
                    static bool wasPressed = false;
                    if (success) {
                        wasPressed = true;
                    } else if (wasPressed) {
                        wasPressed = false;
                        if (currentFan < 5.0) {
                            currentFan = std::min(5.0, currentFan + 1.0);
                            vehicle.setFan(currentFan);
                            vehicleData->fan = currentFan;
                            std::cout << "[Keyboard] Fan speed increased to " << currentFan << std::endl;
                        }
                    }
                }, "increase fan", false);
                
                checkKey(VK_UP, [&](bool success) {
                    static bool wasPressed = false;
                    if (success) {
                        wasPressed = true;
                    } else if (wasPressed && vehicleData->ac_state) {
                        wasPressed = false;
                        double currentTemp = vehicle.getAirConditioningLevel();
                        if (currentTemp < 32.0) {
                            double newTemp = std::min(32.0, currentTemp + 0.5);
                            vehicle.setAirConditioningLevel(newTemp);
                            vehicleData->air_condition = newTemp;
                            std::cout << "[Keyboard] AC temperature increased to " << newTemp << std::endl;
                        }
                    }
                }, "increase AC temp", false);
                
                checkKey(VK_DOWN, [&](bool success) {
                    static bool wasPressed = false;
                    if (success) {
                        wasPressed = true;
                    } else if (wasPressed && vehicleData->ac_state) {
                        wasPressed = false;
                        double currentTemp = vehicle.getAirConditioningLevel();
                        if (currentTemp > 16.0) {
                            double newTemp = std::max(16.0, currentTemp - 0.5);
                            vehicle.setAirConditioningLevel(newTemp);
                            vehicleData->air_condition = newTemp;
                            std::cout << "[Keyboard] AC temperature decreased to " << newTemp << std::endl;
                        }
                    }
                }, "decrease AC temp", false);
                
                checkKey('1', [&](bool success) { 
                    if (success) {
                        std::cout << "[Keyboard] Attempting to change to Normal mode" << std::endl;
                        if (!vehicle.changeDrivingMode(DrivingModeType::NORMAL)) {
                            std::cout << "[Keyboard] Cannot change to Normal mode: Speed too high" << std::endl;
                        }
                    }
                }, "normal mode", false);
                checkKey('2', [&](bool success) { 
                    if (success) {
                        std::cout << "[Keyboard] Attempting to change to Sport mode" << std::endl;
                        if (!vehicle.changeDrivingMode(DrivingModeType::SPORT)) {
                            std::cout << "[Keyboard] Cannot change to Sport mode: Speed too high" << std::endl;
                        }
                    }
                }, "sport mode", false);
                checkKey('3', [&](bool success) { 
                    if (success) {
                        std::cout << "[Keyboard] Attempting to change to Eco mode" << std::endl;
                        if (!vehicle.changeDrivingMode(DrivingModeType::ECO)) {
                            std::cout << "[Keyboard] Cannot change to Eco mode: Speed too high" << std::endl;
                        }
                    }
                }, "eco mode", false);
                checkKey('4', [&](bool success) { 
                    if (success) {
                        if (!vehicle.changeDrivingMode(DrivingModeType::SNOW_OFFROAD)) {
                            std::cout << "[Keyboard] Cannot change to Snow/Offroad mode: Speed too high" << std::endl;
                        }
                    }
                }, "snow/offroad mode", false);
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
                    vehicleData->gear = vehicle.getGear();
                    vehicleData->mode = vehicle.getDrivingMode().getCurrentModeString();
                    
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
                    vehicleData->air_condition = vehicle.getAirConditioningLevel();
                    vehicleData->altitude = environment.getAltitude();
                    vehicleData->temperature = environment.getTemperature();
                    vehicleData->weather = environment.getWeatherCondition();
                    vehicleData->wind = environment.getWindSpeed();
                    
                    // Update timestamp and warning
                    vehicleData->timestamp_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::system_clock::now().time_since_epoch()).count();
                    vehicleData->warning = vehicle.getWarningMessages();

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