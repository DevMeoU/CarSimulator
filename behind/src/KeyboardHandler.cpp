#include "ThreadHandlers.h"
#include <Windows.h>
#include <iostream>

void KeyboardHandler::threadFunction() {
    std::cout << "[Keyboard] Thread started" << std::endl;
    
    static std::unordered_map<int, bool> keyStates;
    static auto lastUpdate = std::chrono::steady_clock::now();

    while (running) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate);
        
        if (elapsed.count() >= 50) {
            lastUpdate = now;
            
            auto checkKey = [&](int key, auto action) {
                bool currentState = (GetAsyncKeyState(key) & 0x8000) != 0;
                if (currentState && !keyStates[key]) {
                    std::cout << "[Keyboard] Key pressed: " << static_cast<char>(key) << std::endl;
                    action();
                }
                keyStates[key] = currentState;
            };
            
            checkKey(VK_SPACE, [&]() { vehicle.accelerate(0.5); });
            checkKey(VK_RETURN, [&]() { vehicle.brake(0.5); });
            checkKey('D', [&]() { vehicle.setGear("D"); });
            checkKey('R', [&]() { vehicle.setGear("R"); });
            checkKey('N', [&]() { vehicle.setGear("N"); });
            checkKey('P', [&]() { vehicle.setParking(true); });
            checkKey('V', [&]() { vehicle.setLeftSignalOn(true); });
            checkKey('B', [&]() { vehicle.setRightSignalOn(true); });
        }
        
        double deltaTime = elapsed.count() / 1000.0;
        vehicle.update(deltaTime);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    std::cout << "[Keyboard] Thread stopped" << std::endl;
}