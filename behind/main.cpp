#include <iostream>
#include <atomic>
#include <thread>
#include <winsock2.h>
#include <Windows.h>
#include "include/Vehicle.h"
#include "../server/cpp-httplib/httplib.h"
#include "include/external/json/json.hpp"

using namespace std;

std::atomic<bool> running{true};

void keyboard_handler(Vehicle& vehicle) {
    while (running) {
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
            vehicle.accelerate();
        }
        if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
            vehicle.brake();
        }
        if (GetAsyncKeyState('D') & 0x8000) vehicle.setGear("D");
        if (GetAsyncKeyState('R') & 0x8000) vehicle.setGear("R");
        if (GetAsyncKeyState('N') & 0x8000) vehicle.setGear("N");
        if (GetAsyncKeyState('P') & 0x8000) vehicle.setParking(true);
        this_thread::sleep_for(chrono::milliseconds(10));
    }
}

void http_client(Vehicle& vehicle) {
    httplib::Client cli("localhost", 8080);
    while (running) {
        json data = {
            {"speed", vehicle.getSpeed()},
            {"mode", vehicle.getDrivingMode()},
            {"battery", vehicle.getBatteryLevel()},
            {"gear", vehicle.getCurrentGear()},
            {"park", vehicle.isParking()}
        };
        cli.Post("/data", data.dump(), "application/json");
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    Vehicle vehicle;
    
    thread keyboard_thread(keyboard_handler, ref(vehicle));
    thread http_thread(http_client, ref(vehicle));

    keyboard_thread.join();
    http_thread.join();

    WSACleanup();
    return 0;
}