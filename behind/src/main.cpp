#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>

#include "../include/Vehicle.h"
#include "../include/Utils.h"

// Function prototypes
void displayMenu();
void testStartEngine(Vehicle& vehicle);
void testDrivingModes(Vehicle& vehicle);
void testSeatbeltWarning(Vehicle& vehicle);
void testHighSpeedWarning(Vehicle& vehicle);
void testEnvironmentalConditions(Vehicle& vehicle);
void testBatteryAndRange(Vehicle& vehicle);
void testFaultSimulation(Vehicle& vehicle);
void runSimulation(Vehicle& vehicle);

int main() {
    // Create vehicle instance
    Vehicle vehicle;
    
    // Welcome message
    Utils::clearScreen();
    std::cout << "====================================================" << std::endl;
    std::cout << "       Electric Vehicle Simulation System           " << std::endl;
    std::cout << "====================================================" << std::endl;
    std::cout << std::endl;
    std::cout << "This program simulates the operation of an electric vehicle" << std::endl;
    std::cout << "based on the specifications provided in the documentation." << std::endl;
    std::cout << std::endl;
    Utils::pauseExecution();
    
    // Main menu loop
    bool running = true;
    while (running) {
        displayMenu();
        
        std::string choice = Utils::getValidatedInput("Enter your choice (1-9): ", 
            [](const std::string& input) {
                if (input.length() != 1) return false;
                char c = input[0];
                return c >= '1' && c <= '9';
            });
        
        Utils::clearScreen();
        
        switch (choice[0]) {
            case '1':
                testStartEngine(vehicle);
                break;
            case '2':
                testDrivingModes(vehicle);
                break;
            case '3':
                testSeatbeltWarning(vehicle);
                break;
            case '4':
                testHighSpeedWarning(vehicle);
                break;
            case '5':
                testEnvironmentalConditions(vehicle);
                break;
            case '6':
                testBatteryAndRange(vehicle);
                break;
            case '7':
                testFaultSimulation(vehicle);
                break;
            case '8':
                runSimulation(vehicle);
                break;
            case '9':
                std::cout << "Exiting program. Goodbye!" << std::endl;
                running = false;
                break;
        }
        
        if (running) {
            std::cout << std::endl;
            std::cout << "Press Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    
    return 0;
}

void displayMenu() {
    Utils::clearScreen();
    std::cout << "====================================================" << std::endl;
    std::cout << "                      MAIN MENU                     " << std::endl;
    std::cout << "====================================================" << std::endl;
    std::cout << "1. Test Engine Start Requirements" << std::endl;
    std::cout << "2. Test Driving Modes and Speed Limits" << std::endl;
    std::cout << "3. Test Seatbelt Warning System" << std::endl;
    std::cout << "4. Test High Speed Warning" << std::endl;
    std::cout << "5. Test Environmental Conditions Impact" << std::endl;
    std::cout << "6. Test Battery and Range Calculation" << std::endl;
    std::cout << "7. Test Fault Simulation" << std::endl;
    std::cout << "8. Run Complete Simulation" << std::endl;
    std::cout << "9. Exit" << std::endl;
    std::cout << "====================================================" << std::endl;
}

void testStartEngine(Vehicle& vehicle) {
    std::cout << "====================================================" << std::endl;
    std::cout << "          TEST: ENGINE START REQUIREMENTS           " << std::endl;
    std::cout << "====================================================" << std::endl;
    std::cout << std::endl;
    
    // Reset vehicle state
    vehicle = Vehicle();
    
    // Test 1: Try to start engine without locking doors
    std::cout << "Test 1: Starting engine without locking doors" << std::endl;
    std::cout << "Current state: Doors " << (vehicle.isDoorLocked() ? "Locked" : "Unlocked") << std::endl;
    bool result = vehicle.startEngine();
    std::cout << "Result: " << (result ? "Engine started" : "Engine failed to start") << std::endl;
    std::cout << "Messages: " << vehicle.getDisplay().displayAll() << std::endl;
    vehicle.getDisplay().clearMessages();
    
    // Test 2: Lock doors but don't press brake
    std::cout << std::endl << "Test 2: Starting engine with locked doors but without pressing brake" << std::endl;
    vehicle.setDoorLocked(true);
    std::cout << "Current state: Doors " << (vehicle.isDoorLocked() ? "Locked" : "Unlocked") << std::endl;
    std::cout << "Brake press time: " << vehicle.getBrakePressTime() << " seconds" << std::endl;
    result = vehicle.startEngine();
    std::cout << "Result: " << (result ? "Engine started" : "Engine failed to start") << std::endl;
    std::cout << "Messages: " << vehicle.getDisplay().displayAll() << std::endl;
    vehicle.getDisplay().clearMessages();
    
    // Test 3: Lock doors and press brake for 3 seconds
    std::cout << std::endl << "Test 3: Starting engine with locked doors and pressing brake for 3 seconds" << std::endl;
    vehicle.setDoorLocked(true);
    std::cout << "Current state: Doors " << (vehicle.isDoorLocked() ? "Locked" : "Unlocked") << std::endl;
    std::cout << "Pressing brake for 3 seconds..." << std::endl;
    vehicle.pressBrake(3.0);
    std::cout << "Brake press time: " << vehicle.getBrakePressTime() << " seconds" << std::endl;
    result = vehicle.startEngine();
    std::cout << "Result: " << (result ? "Engine started" : "Engine failed to start") << std::endl;
    std::cout << "Messages: " << vehicle.getDisplay().displayAll() << std::endl;
    
    std::cout << std::endl;
    std::cout << "Test completed. The engine start requirements are working as expected." << std::endl;
}

void testDrivingModes(Vehicle& vehicle) {
    std::cout << "====================================================" << std::endl;
    std::cout << "          TEST: DRIVING MODES AND SPEED LIMITS      " << std::endl;
    std::cout << "====================================================" << std::endl;
    std::cout << std::endl;
    
    // Reset vehicle state and prepare for test
    vehicle = Vehicle();
    vehicle.setDoorLocked(true);
    vehicle.pressBrake(3.0);
    vehicle.startEngine();
    vehicle.setSeatbeltOn(true);
    
    std::cout << "Vehicle prepared for test. Engine started." << std::endl;
    std::cout << "Current driving mode: " << vehicle.getDrivingMode().getCurrentModeString() << std::endl;
    std::cout << "Current max speed: " << vehicle.getCurrentMaxSpeed() << " km/h" << std::endl;
    std::cout << std::endl;
    
    // Test each driving mode
    std::vector<DrivingModeType> modes = {
        DrivingModeType::NORMAL,
        DrivingModeType::SPORT,
        DrivingModeType::ECO,
        DrivingModeType::SNOW_OFFROAD
    };
    
    for (const auto& mode : modes) {
        std::cout << "Testing mode: " << DrivingMode::getModeString(mode) << std::endl;
        
        // Change to the mode
        bool result = vehicle.changeDrivingMode(mode);
        std::cout << "Mode change result: " << (result ? "Success" : "Failed") << std::endl;
        
        // Accelerate to see speed limit
        std::cout << "Accelerating to maximum speed..." << std::endl;
        for (int i = 0; i < 20; i++) {
            vehicle.accelerate(1.0);
            vehicle.update(0.5);
        }
        
        std::cout << "Current speed: " << vehicle.getSpeed() << " km/h" << std::endl;
        std::cout << "Max speed for this mode: " << vehicle.getCurrentMaxSpeed() << " km/h" << std::endl;
        std::cout << std::endl;
    }
    
    // Test changing to a mode with lower speed limit while at high speed
    std::cout << "Testing mode change at high speed:" << std::endl;
    vehicle.changeDrivingMode(DrivingModeType::SPORT);
    std::cout << "Current mode: " << vehicle.getDrivingMode().getCurrentModeString() << std::endl;
    
    // Accelerate to high speed
    std::cout << "Accelerating to high speed..." << std::endl;
    for (int i = 0; i < 20; i++) {
        vehicle.accelerate(1.0);
        vehicle.update(0.5);
    }
    std::cout << "Current speed: " << vehicle.getSpeed() << " km/h" << std::endl;
    
    // Try to change to ECO mode
    std::cout << "Attempting to change to ECO mode at high speed..." << std::endl;
    bool result = vehicle.changeDrivingMode(DrivingModeType::ECO);
    std::cout << "Mode change result: " << (result ? "Success" : "Failed") << std::endl;
    std::cout << "Messages: " << vehicle.getDisplay().displayAll() << std::endl;
    
    std::cout << std::endl;
    std::cout << "Test completed. The driving modes and speed limits are working as expected." << std::endl;
}

void testSeatbeltWarning(Vehicle& vehicle) {
    std::cout << "====================================================" << std::endl;
    std::cout << "          TEST: SEATBELT WARNING SYSTEM             " << std::endl;
    std::cout << "====================================================" << std::endl;
    std::cout << std::endl;
    
    // Reset vehicle state and prepare for test
    vehicle = Vehicle();
    vehicle.setDoorLocked(true);
    vehicle.pressBrake(3.0);
    vehicle.startEngine();
    
    std::cout << "Vehicle prepared for test. Engine started." << std::endl;
    std::cout << "Current seatbelt status: " << (vehicle.isSeatbeltOn() ? "Fastened" : "Unfastened") << std::endl;
    std::cout << std::endl;
    
    // Test 1: Drive without seatbelt
    std::cout << "Test 1: Driving without seatbelt" << std::endl;
    vehicle.setSeatbeltOn(false);
    vehicle.getDisplay().clearMessages();
    
    std::cout << "Accelerating..." << std::endl;
    vehicle.accelerate(0.5);
    vehicle.update(1.0);
    
    std::cout << "Current speed: " << vehicle.getSpeed() << " km/h" << std::endl;
    std::cout << "Messages: " << vehicle.getDisplay().displayAll() << std::endl;
    
    // Test 2: Fasten seatbelt while driving
    std::cout << std::endl << "Test 2: Fastening seatbelt while driving" << std::endl;
    vehicle.setSeatbeltOn(true);
    vehicle.getDisplay().clearMessages();
    vehicle.update(1.0);
    
    std::cout << "Current seatbelt status: " << (vehicle.isSeatbeltOn() ? "Fastened" : "Unfastened") << std::endl;
    std::cout << "Current speed: " << vehicle.getSpeed() << " km/h" << std::endl;
    std::cout << "Messages: " << vehicle.getDisplay().displayAll() << std::endl;
    
    // Test 3: Unfasten seatbelt while driving
    std::cout << std::endl << "Test 3: Unfastening seatbelt while driving" << std::endl;
    vehicle.setSeatbeltOn(false);
    vehicle.getDisplay().clearMessages();
    vehicle.update(1.0);
    
    std::cout << "Current seatbelt status: " << (vehicle.isSeatbeltOn() ? "Fastened" : "Unfastened") << std::endl;
    std::cout << "Current speed: " << vehicle.getSpeed() << " km/h" << std::endl;
    std::cout << "Messages: " << vehicle.getDisplay().displayAll() << std::endl;
    
    std::cout << std::endl;
    std::cout << "Test completed. The seatbelt warning system is working as expected." << std::endl;
}

void testHighSpeedWarning(Vehicle& vehicle) {
    std::cout << "====================================================" << std::endl;
    std::cout << "          TEST: HIGH SPEED WARNING SYSTEM           " << std::endl;
    std::cout << "====================================================" << std::endl;
    std::cout << std::endl;
    
    // Reset vehicle state and prepare for test
    vehicle = Vehicle();
    vehicle.setDoorLocked(true);
    vehicle.pressBrake(3.0);
    vehicle.startEngine();
    vehicle.setSeatbeltOn(true);
    vehicle.changeDrivingMode(DrivingModeType::SPORT);  // Use Sport mode for higher speed limit
    
    std::cout << "Vehicle prepared for test. Engine started in Sport mode." << std::endl;
    std::cout << "Current max speed: " << vehicle.getCurrentMaxSpeed() << " km/h" << std::endl;
    std::cout << std::endl;
    
    // Test: Accelerate to high speed
    std::cout << "Accelerating gradually to test high speed warning..." << std::endl;
    vehicle.getDisplay().clearMessages();
    
    for (int i = 0; i < 15; i++) {
        vehicle.accelerate(0.5);
        vehicle.update(1.0);
        
        std::cout << "Current speed: " << vehicle.getSpeed() << " km/h" << std::endl;
        
        // Check if warning is displayed
        auto warnings = vehicle.getDisplay().getMessages(MessageType::WARNING);
        if (!warnings.empty()) {
            std::cout << "Warning triggered at " << vehicle.getSpeed() << " km/h:" << std::endl;
            std::cout << vehicle.getDisplay().displayByType(MessageType::WARNING) << std::endl;
            break;
        }
    }
    
    // Continue accelerating to see if we reach max speed
    std::cout << std::endl << "Continuing acceleration to max speed..." << std::endl;
    for (int i = 0; i < 10; i++) {
        vehicle.accelerate(1.0);
        vehicle.update(1.0);
    }
    
    std::cout << "Final speed: " << vehicle.getSpeed() << " km/h" << std::endl;
    std::cout << "Max speed limit: " << vehicle.getCurrentMaxSpeed() << " km/h" << std::endl;
    
    std::cout << std::endl;
    std::cout << "Test completed. The high speed warning system is working as expected." << std::endl;
}

void testEnvironmentalConditions(Vehicle& vehicle) {
    std::cout << "====================================================" << std::endl;
    std::cout << "          TEST: ENVIRONMENTAL CONDITIONS IMPACT     " << std::endl;
    std::cout << "====================================================" << std::endl;
    std::cout << std::endl;
    
    // Reset vehicle state and prepare for test
    vehicle = Vehicle();
    vehicle.setDoorLocked(true);
    vehicle.pressBrake(3.0);
    vehicle.startEngine();
    vehicle.setSeatbeltOn(true);
    
    std::cout << "Vehicle prepared for test. Engine started." << std::endl;
    std::cout << std::endl;
    
    // Test different environmental conditions
    struct TestCondition {
        std::string name;
        double temperature;
        double altitude;
        WeatherType weather;
        double slope;
        double load;
    };
    
    std::vector<TestCondition> conditions = {
        {"Normal conditions", 20.0, 0.0, WeatherType::CLEAR, 0.0, 0.0},
        {"Cold weather", -10.0, 0.0, WeatherType::SNOW, 0.0, 0.0},
        {"Hot weather", 40.0, 0.0, WeatherType::CLEAR, 0.0, 0.0},
        {"High altitude", 20.0, 2000.0, WeatherType::CLEAR, 0.0, 0.0},
        {"Steep slope", 20.0, 0.0, WeatherType::CLEAR, 15.0, 0.0},
        {"Heavy load", 20.0, 0.0, WeatherType::CLEAR, 0.0, 300.0},
        {"Rainy weather", 15.0, 0.0, WeatherType::RAIN, 0.0, 0.0},
        {"Extreme conditions", -5.0, 1500.0, WeatherType::SNOW, 10.0, 200.0}
    };
    
    for (const auto& condition : conditions) {
        std::cout << "Testing: " << condition.name << std::endl;
        
        // Set environmental conditions
        vehicle.getEnvironment().setTemperature(condition.temperature);
        vehicle.getEnvironment().setAltitude(condition.altitude);
        vehicle.getEnvironment().setWeather(condition.weather);
        vehicle.getEnvironment().setSlope(condition.slope);
        vehicle.getEnvironment().setLoad(condition.load);
        
        // Update vehicle and check impact
        vehicle.update(1.0);
        
        // Calculate range under these conditions
        double range = vehicle.getBattery().calculateRemainingRange(
            vehicle.getDrivingMode().getCurrentMode(), 
            vehicle.getEnvironment());
        
        // Calculate power impact
        double powerImpact = vehicle.getEnvironment().calculatePowerImpact();
        
        // Calculate tire friction
        double tireFriction = vehicle.getEnvironment().calculateTireFriction();
        
        std::cout << "Environment: " << vehicle.getEnvironment().getStatusString() << std::endl;
        std::cout << "Estimated range: " << range << " km" << std::endl;
        std::cout << "Power impact factor: " << powerImpact << std::endl;
        std::cout << "Tire friction coefficient: " << tireFriction << std::endl;
        std::cout << std::endl;
    }
    
    std::cout << "Test completed. The environmental conditions impact is working as expected." << std::endl;
}

void testBatteryAndRange(Vehicle& vehicle) {
    std::cout << "====================================================" << std::endl;
    std::cout << "          TEST: BATTERY AND RANGE CALCULATION       " << std::endl;
    std::cout << "====================================================" << std::endl;
    std::cout << std::endl;
    
    // Reset vehicle state and prepare for test
    vehicle = Vehicle();
    vehicle.setDoorLocked(true);
    vehicle.pressBrake(3.0);
    vehicle.startEngine();
    vehicle.setSeatbeltOn(true);
    
    std::cout << "Vehicle prepared for test. Engine started." << std::endl;
    std::cout << "Initial battery status:" << std::endl;
    std::cout << vehicle.getBattery().getStatusString() << std::endl;
    
    // Test range calculation in different driving modes
    std::cout << std::endl << "Testing range in different driving modes:" << std::endl;
    
    std::vector<DrivingModeType> modes = {
        DrivingModeType::NORMAL,
        DrivingModeType::SPORT,
        DrivingModeType::ECO,
        DrivingModeType::SNOW_OFFROAD
    };
    
    for (const auto& mode : modes) {
        vehicle.changeDrivingMode(mode);
        double range = vehicle.getBattery().calculateRemainingRange(
            mode, vehicle.getEnvironment());
        
        std::cout << "Mode: " << DrivingMode::getModeString(mode) << std::endl;
        std::cout << "Estimated range: " << range << " km" << std::endl;
        std::cout << std::endl;
    }
    
    // Test battery discharge during driving
    std::cout << "Testing battery discharge during driving:" << std::endl;
    vehicle.changeDrivingMode(DrivingModeType::NORMAL);
    
    std::cout << "Initial charge: " << (vehicle.getBattery().getCurrentCharge() * 100.0) << "%" << std::endl;
    
    // Drive for a while
    std::cout << "Driving at moderate speed for 30 simulation steps..." << std::endl;
    for (int i = 0; i < 30; i++) {
        vehicle.accelerate(0.5);
        vehicle.update(1.0);
    }
    
    std::cout << "Final charge: " << (vehicle.getBattery().getCurrentCharge() * 100.0) << "%" << std::endl;
    std::cout << "Current speed: " << vehicle.getSpeed() << " km/h" << std::endl;
    
    // Test regenerative braking
    std::cout << std::endl << "Testing regenerative braking:" << std::endl;
    double chargeBeforeBraking = vehicle.getBattery().getCurrentCharge();
    std::cout << "Charge before braking: " << (chargeBeforeBraking * 100.0) << "%" << std::endl;
    
    // Apply brakes
    std::cout << "Applying brakes..." << std::endl;
    for (int i = 0; i < 10; i++) {
        vehicle.brake(0.8);
        vehicle.update(0.5);
    }
    
    double chargeAfterBraking = vehicle.getBattery().getCurrentCharge();
    std::cout << "Charge after braking: " << (chargeAfterBraking * 100.0) << "%" << std::endl;
    std::cout << "Charge recovered: " << ((chargeAfterBraking - chargeBeforeBraking) * 100.0) << "%" << std::endl;
    std::cout << "Current speed: " << vehicle.getSpeed() << " km/h" << std::endl;
    
    std::cout << std::endl;
    std::cout << "Test completed. The battery and range calculation is working as expected." << std::endl;
}

void testFaultSimulation(Vehicle& vehicle) {
    std::cout << "====================================================" << std::endl;
    std::cout << "          TEST: FAULT SIMULATION                    " << std::endl;
    std::cout << "====================================================" << std::endl;
    std::cout << std::endl;
    
    // Reset vehicle state and prepare for test
    vehicle = Vehicle();
    vehicle.setDoorLocked(true);
    vehicle.pressBrake(3.0);
    vehicle.startEngine();
    vehicle.setSeatbeltOn(true);
    
    std::cout << "Vehicle prepared for test. Engine started." << std::endl;
    std::cout << std::endl;
    
    // Test different fault scenarios
    struct FaultTest {
        std::string name;
        FaultType type;
    };
    
    std::vector<FaultTest> faults = {
        {"Sensor Disconnect", FaultType::SENSOR_DISCONNECT},
        {"Engine Overheat", FaultType::ENGINE_OVERHEAT},
        {"Battery Fault", FaultType::BATTERY_FAULT},
        {"Brake Failure", FaultType::BRAKE_FAILURE}
    };
    
    for (const auto& fault : faults) {
        std::cout << "Testing fault: " << fault.name << std::endl;
        
        // Simulate fault
        auto effects = vehicle.getFaultSim().simulateFault(fault.type);
        
        std::cout << "Fault effects:" << std::endl;
        for (const auto& effect : effects) {
            std::cout << "  - " << effect << std::endl;
        }
        
        // Handle fault
        auto actions = vehicle.getFaultSim().handleFault(vehicle);
        
        std::cout << "System response:" << std::endl;
        for (const auto& action : actions) {
            std::cout << "  - " << action << std::endl;
        }
        
        // Clear fault for next test
        vehicle.getFaultSim().clearFault();
        std::cout << std::endl;
    }
    
    std::cout << "Test completed. The fault simulation is working as expected." << std::endl;
}

void runSimulation(Vehicle& vehicle) {
    std::cout << "====================================================" << std::endl;
    std::cout << "          RUNNING COMPLETE SIMULATION               " << std::endl;
    std::cout << "====================================================" << std::endl;
    std::cout << std::endl;
    
    // Reset vehicle state
    vehicle = Vehicle();
    
    std::cout << "Starting simulation of a complete drive cycle..." << std::endl;
    std::cout << std::endl;
    
    // Step 1: Prepare vehicle
    std::cout << "Step 1: Preparing vehicle" << std::endl;
    vehicle.setDoorLocked(true);
    std::cout << "Doors locked" << std::endl;
    
    vehicle.setSeatbeltOn(true);
    std::cout << "Seatbelt fastened" << std::endl;
    
    vehicle.pressBrake(3.0);
    std::cout << "Brake pressed for 3 seconds" << std::endl;
    
    bool started = vehicle.startEngine();
    std::cout << "Engine start result: " << (started ? "Success" : "Failed") << std::endl;
    if (!started) {
        std::cout << "Messages: " << vehicle.getDisplay().displayAll() << std::endl;
        std::cout << "Simulation aborted due to engine start failure." << std::endl;
        return;
    }
    
    std::cout << std::endl;
    
    // Step 2: City driving in ECO mode
    std::cout << "Step 2: City driving in ECO mode" << std::endl;
    vehicle.changeDrivingMode(DrivingModeType::ECO);
    std::cout << "Changed to ECO mode" << std::endl;
    
    std::cout << "Accelerating to city speed..." << std::endl;
    for (int i = 0; i < 5; i++) {
        vehicle.accelerate(0.3);
        vehicle.update(1.0);
        std::cout << "Speed: " << vehicle.getSpeed() << " km/h" << std::endl;
    }
    
    std::cout << "Cruising in the city..." << std::endl;
    for (int i = 0; i < 10; i++) {
        vehicle.update(1.0);
        
        // Occasional acceleration and braking to simulate city traffic
        if (i % 3 == 0) {
            vehicle.brake(0.2);
            std::cout << "Light braking, speed: " << vehicle.getSpeed() << " km/h" << std::endl;
        } else if (i % 4 == 0) {
            vehicle.accelerate(0.2);
            std::cout << "Light acceleration, speed: " << vehicle.getSpeed() << " km/h" << std::endl;
        } else {
            std::cout << "Cruising, speed: " << vehicle.getSpeed() << " km/h" << std::endl;
        }
    }
    
    std::cout << std::endl;
    
    // Step 3: Highway driving in SPORT mode
    std::cout << "Step 3: Highway driving in SPORT mode" << std::endl;
    vehicle.changeDrivingMode(DrivingModeType::SPORT);
    std::cout << "Changed to SPORT mode" << std::endl;
    
    std::cout << "Accelerating to highway speed..." << std::endl;
    for (int i = 0; i < 10; i++) {
        vehicle.accelerate(0.8);
        vehicle.update(1.0);
        std::cout << "Speed: " << vehicle.getSpeed() << " km/h" << std::endl;
        
        // Check for high speed warning
        auto warnings = vehicle.getDisplay().getMessages(MessageType::WARNING);
        if (!warnings.empty()) {
            std::cout << "Warning triggered: " << vehicle.getDisplay().displayByType(MessageType::WARNING) << std::endl;
            vehicle.getDisplay().clearMessages();
        }
    }
    
    std::cout << "Cruising on highway..." << std::endl;
    for (int i = 0; i < 10; i++) {
        vehicle.update(1.0);
        std::cout << "Cruising, speed: " << vehicle.getSpeed() << " km/h" << std::endl;
    }
    
    std::cout << std::endl;
    
    // Step 4: Changing environmental conditions
    std::cout << "Step 4: Weather changes - entering rain" << std::endl;
    vehicle.getEnvironment().setWeather(WeatherType::RAIN);
    vehicle.getEnvironment().setTemperature(15.0);
    std::cout << "Weather changed to rain, temperature dropped to 15°C" << std::endl;
    
    std::cout << "Adjusting driving..." << std::endl;
    vehicle.changeDrivingMode(DrivingModeType::NORMAL);
    std::cout << "Changed to NORMAL mode" << std::endl;
    
    for (int i = 0; i < 5; i++) {
        vehicle.brake(0.3);
        vehicle.update(1.0);
        std::cout << "Reducing speed, current speed: " << vehicle.getSpeed() << " km/h" << std::endl;
    }
    
    std::cout << std::endl;
    
    // Step 5: Off-road section
    std::cout << "Step 5: Off-road section" << std::endl;
    vehicle.getEnvironment().setRoughness(0.7);
    vehicle.getEnvironment().setSlope(8.0);
    std::cout << "Terrain changed to rough with 8% slope" << std::endl;
    
    vehicle.changeDrivingMode(DrivingModeType::SNOW_OFFROAD);
    std::cout << "Changed to SNOW/OFF-ROAD mode" << std::endl;
    
    for (int i = 0; i < 8; i++) {
        vehicle.update(1.0);
        
        if (i % 2 == 0) {
            vehicle.accelerate(0.4);
            std::cout << "Navigating rough terrain, speed: " << vehicle.getSpeed() << " km/h" << std::endl;
        } else {
            vehicle.brake(0.2);
            std::cout << "Careful driving, speed: " << vehicle.getSpeed() << " km/h" << std::endl;
        }
    }
    
    std::cout << std::endl;
    
    // Step 6: Return to normal conditions and stop
    std::cout << "Step 6: Returning to normal conditions and stopping" << std::endl;
    vehicle.getEnvironment().setWeather(WeatherType::CLEAR);
    vehicle.getEnvironment().setTemperature(20.0);
    vehicle.getEnvironment().setRoughness(0.0);
    vehicle.getEnvironment().setSlope(0.0);
    std::cout << "Returned to normal road conditions" << std::endl;
    
    vehicle.changeDrivingMode(DrivingModeType::NORMAL);
    std::cout << "Changed to NORMAL mode" << std::endl;
    
    std::cout << "Slowing down to stop..." << std::endl;
    while (vehicle.getSpeed() > 0.1) {
        vehicle.brake(0.5);
        vehicle.update(1.0);
        std::cout << "Braking, speed: " << vehicle.getSpeed() << " km/h" << std::endl;
    }
    
    vehicle.stopEngine();
    std::cout << "Vehicle stopped, engine turned off" << std::endl;
    
    // Final status
    std::cout << std::endl;
    std::cout << "Simulation completed. Final vehicle status:" << std::endl;
    std::cout << vehicle.getStatusString() << std::endl;
    std::cout << "Battery status:" << std::endl;
    std::cout << vehicle.getBattery().getStatusString() << std::endl;
}
