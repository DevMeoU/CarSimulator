#include <winsock2.h>
#include <Windows.h>
#include <iostream>
#include <thread>
#include <atomic>
#include <string>
#include <chrono>
#include <cstdlib>
#include "../../keyboard/keycodes.hpp"
#include "../system/windows/queue/SynchronizedQueue.hpp"
#include "../../server/cpp-httplib/httplib.h"

namespace car_sim {

struct CarState {
    // Thông số vận hành
    int speed;
    std::string mode;
    int battery;
    int battery_temp;
    std::string gear;
    
    // Thông số quãng đường
    int estimated_distance;
    int distance_traveled;
    
    // Đèn báo
    int signal_left;
    int signal_right;
    
    // Điều hòa
    int wind;
    int air_condition;
    
    // Trạng thái xe
    int plug_in;
    int door_lock;
    int seat_belt;
    int park;
    int brake;
    int gas;
    std::string warning;

    CarState() {
        speed = 0;
        mode = "";
        battery = 0;
        battery_temp = 0;
        gear = "";
        estimated_distance = 0;
        distance_traveled = 0;
        signal_left = 0;
        signal_right = 0;
        wind = 0;
        air_condition = 0;
        plug_in = 0;
        door_lock = 0;
        seat_belt = 0;
        park = 0;
        brake = 0;
        gas = 0;
        warning = "";
    }

    bool empty() const {
        return 
            speed == 0 &&
            mode.empty() &&
            battery == 0 &&
            battery_temp == 0 &&
            gear.empty() &&
            estimated_distance == 0 &&
            distance_traveled == 0 &&
            signal_left == 0 &&
            signal_right == 0 &&
            wind == 0 &&
            air_condition == 0 &&
            plug_in == 0 &&
            door_lock == 0 &&
            seat_belt == 0 &&
            park == 0 &&
            brake == 0 &&
            gas == 0 &&
            warning.empty();
    }

    void clear() {
        speed = battery = battery_temp = estimated_distance = distance_traveled = signal_left = signal_right = wind = air_condition = plug_in = door_lock = seat_belt = park = brake = gas = 0;
        mode.clear();
        gear.clear();
        warning.clear();
    }

    int& operator[](const std::string& key) {
        if (key == "speed") return speed;
        if (key == "battery") return battery;
        if (key == "battery_temp") return battery_temp;
        if (key == "estimated_distance") return estimated_distance;
        if (key == "distance_traveled") return distance_traveled;
        if (key == "signal_left") return signal_left;
        if (key == "signal_right") return signal_right;
        if (key == "wind") return wind;
        if (key == "air_condition") return air_condition;
        if (key == "plug_in") return plug_in;
        if (key == "door_lock") return door_lock;
        if (key == "seat_belt") return seat_belt;
        if (key == "park") return park;
        if (key == "brake") return brake;
        if (key == "gas") return gas;
        throw std::out_of_range("Invalid key: " + key);
    }

    std::string to_string() const {
        std::string result = "{";
        result += "\"speed\":" + std::to_string(speed) + ",";
        result += "\"mode\":\"" + mode + "\",";
        result += "\"battery\":" + std::to_string(battery) + ",";
        result += "\"battery_temp\":" + std::to_string(battery_temp) + ",";
        result += "\"gear\":\"" + gear + "\",";
        result += "\"estimated_distance\":" + std::to_string(estimated_distance) + ",";
        result += "\"distance_traveled\":" + std::to_string(distance_traveled) + ",";
        result += "\"signal_left\":" + std::to_string(signal_left) + ",";
        result += "\"signal_right\":" + std::to_string(signal_right) + ",";
        result += "\"wind\":" + std::to_string(wind) + ",";
        result += "\"air_condition\":" + std::to_string(air_condition) + ",";
        result += "\"plug_in\":" + std::to_string(plug_in) + ",";
        result += "\"door_lock\":" + std::to_string(door_lock) + ",";
        result += "\"seat_belt\":" + std::to_string(seat_belt) + ",";
        result += "\"park\":" + std::to_string(park) + ",";
        result += "\"brake\":" + std::to_string(brake) + ",";
        result += "\"gas\":" + std::to_string(gas) + ",";
        result += "\"warning\":\"" + warning + "\"";
        result += "}";
        return result;
    }
};

} // namespace car_sim

using CarStateData = car_sim::CarState;

std::atomic<bool> running{true};
static car_system::windows::queue::SynchronizedQueue<car_sim::CarState> data_queue(10);

void keyboard_thread() {
    while (running) {
        static CarStateData car_state;
        static int speed_value = 0;
        car_state.clear();
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
            car_state.gas = 1;
            speed_value = std::min(200, speed_value + 1);
            car_state.speed = speed_value;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        } else if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
            car_state.brake = 1;
            if (speed_value >= 0) {
                speed_value = std::max(0, speed_value - 1);
                car_state.speed = speed_value;
            }
        } else {
            if (speed_value > 0) {
                speed_value = std::max(0, speed_value - 1);
                car_state.speed = speed_value;
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        }
        if (GetAsyncKeyState('H') & 0x8000) car_state.park = 1;
        if (GetAsyncKeyState('J') & 0x8000) car_state.seat_belt = 1;
        if (GetAsyncKeyState('K') & 0x8000) car_state.door_lock = 1;
        if (GetAsyncKeyState('L') & 0x8000) car_state.plug_in = 1;
        if (GetAsyncKeyState('I') & 0x8000) car_state.air_condition = 1;
        if (GetAsyncKeyState(VK_LEFT) & 0x8000) car_state.wind += 1;
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000) car_state.wind -= 1;
        if (GetAsyncKeyState('B') & 0x8000) car_state.signal_left = 1;
        if (GetAsyncKeyState('V') & 0x8000) car_state.signal_right = 1;
        if (GetAsyncKeyState('N') & 0x8000) car_state.gear = "N";
        if (GetAsyncKeyState('D') & 0x8000) car_state.gear = "D";
        if (GetAsyncKeyState('R') & 0x8000) car_state.gear = "R";
        if (GetAsyncKeyState('P') & 0x8000) car_state.gear = "P";
        if (GetAsyncKeyState('M') & 0x8000) car_state.mode = "eco";
        
        if (!car_state.empty()) {
            data_queue.enqueue(car_state);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void process_thread() {
    const std::string SERVER_URL = "http://localhost:8080";
    const int MAX_RETRIES = 3;  // Giảm số lần thử lại
    const int BASE_RETRY_DELAY_MS = 200;  // Giảm thời gian chờ giữa các lần thử
    const int MAX_CONSECUTIVE_FAILURES = 3;  // Giảm số lần thất bại liên tiếp tối đa
    
    httplib::Client cli(SERVER_URL);
    cli.set_connection_timeout(3);  // Giảm thời gian timeout
    cli.set_read_timeout(3);
    cli.set_write_timeout(3);
    cli.set_keep_alive(true);
    
    bool had_error = false;
    int consecutive_failures = 0;
    
    auto check_server = [&cli]() -> bool {
        auto res = cli.Get("/");
        return res && (res->status == 200 || res->status == 404);
    };
    
    while (running) {
        try {
            if (had_error) {
                if (!check_server()) {
                    std::cerr << "Server unavailable. Waiting for connection..." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    continue;
                }
                had_error = false;
                consecutive_failures = 0;
                std::cout << "Successfully reconnected to server!" << std::endl;
            }
            
            while (data_queue.size() > 0 && running) {
                CarStateData data = data_queue.dequeue();
                std::string json_data = data.to_string();
                bool sent = false;
                
                for (int retry = 0; retry < MAX_RETRIES && !sent && running; ++retry) {
                    if (retry > 0) {
                        int delay = BASE_RETRY_DELAY_MS * (1 << retry);
                        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
                        std::cout << "Retrying attempt " << retry + 1 << "..." << std::endl;
                    }
                    
                    auto res = cli.Post("/data", json_data, "application/json");
                    if (res && res->status == 200) {
                        std::cout << "Data sent successfully: " << json_data << std::endl;
                        sent = true;
                    } else {
                        std::cerr << "Error sending data (attempt " << retry + 1 << "/" << MAX_RETRIES
                                  << "): " << (res ? std::to_string(res->status) : "unable to connect") << std::endl;
                    }
                }
                
                if (!sent) {
                    consecutive_failures++;
                    std::cerr << "Failed to send data. Consecutive failures: " << consecutive_failures << std::endl;
                    if (consecutive_failures >= MAX_CONSECUTIVE_FAILURES) {
                        std::cerr << "Too many consecutive failures. Checking connection..." << std::endl;
                        had_error = true;
                        break;
                    }
                    data_queue.enqueue(data);
                }
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(5));  // Giảm thời gian chờ
        } catch (const std::exception& e) {
            if (!had_error) {
                std::cerr << "Error during processing: " << e.what() << std::endl;
                had_error = true;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock" << std::endl;
        return 1;
    }

    std::thread kb_thread(keyboard_thread);
    std::thread proc_thread(process_thread);

    std::cout << "Press 'Q' to quit..." << std::endl;
    while (running) {
        if (GetAsyncKeyState('Q') & 0x8000) {
            running = false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    kb_thread.join();
    proc_thread.join();
    
    WSACleanup();
    return 0;
}