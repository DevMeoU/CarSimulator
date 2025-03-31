#include "../../keyboard/keycodes.hpp"
#include "../system/windows/queue/SynchronizedQueue.hpp"
#include <Windows.h>
#include <iostream>
#include <thread>
#include <atomic>
#include <string>

namespace car_sim {

struct KeyState {
    int gas;
    int brake;
    int forward;
    int backward;
    int left;
    int right;

    KeyState() : gas(0), brake(0), forward(0), backward(0), left(0), right(0) {}

    bool empty() const {
        return !gas && !brake && !forward && !backward && !left && !right;
    }

    void clear() {
        gas = brake = forward = backward = left = right = 0;
    }

    int& operator[](const std::string& key) {
        if (key == "gas") return gas;
        if (key == "brake") return brake;
        if (key == "forward") return forward;
        if (key == "backward") return backward;
        if (key == "left") return left;
        if (key == "right") return right;
        throw std::out_of_range("Invalid key: " + key);
    }

    std::string to_string() const {
        std::string result = "{";
        if (gas) result += "\"gas\":1";
        if (brake) { if (gas) result += ","; result += "\"brake\":1"; }
        if (forward) { if (gas || brake) result += ","; result += "\"forward\":1"; }
        if (backward) { if (gas || brake || forward) result += ","; result += "\"backward\":1"; }
        if (left) { if (gas || brake || forward || backward) result += ","; result += "\"left\":1"; }
        if (right) { if (gas || brake || forward || backward || left) result += ","; result += "\"right\":1"; }
        result += "}";
        return result;
    }
};

} // namespace car_sim

using json = car_sim::KeyState;

std::atomic<bool> running{true};
static car_system::windows::queue::SynchronizedQueue<car_sim::KeyState> data_queue(10);

void keyboard_thread() {
    while (running) {
        json key_data;
        key_data.clear();
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) key_data["gas"] = 1;
        if (GetAsyncKeyState(VK_RETURN) & 0x8000) key_data["brake"] = 1;
        if (GetAsyncKeyState('W') & 0x8000) key_data["forward"] = 1;
        if (GetAsyncKeyState('S') & 0x8000) key_data["backward"] = 1;
        if (GetAsyncKeyState('A') & 0x8000) key_data["left"] = 1;
        if (GetAsyncKeyState('D') & 0x8000) key_data["right"] = 1;
        
        if (!key_data.empty()) {
            data_queue.enqueue(key_data);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void process_thread() {
    while (running) {
        try {
            json data = data_queue.dequeue();
            std::cout << "Key data: " << data.to_string() << std::endl;
        } catch (...) {
            // Handle error
        }
    }
}

int main() {
    std::thread kb_thread(keyboard_thread);
    std::thread proc_thread(process_thread);
    
    while (true) {
        std::cout << "Main thread running..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    
    running = false;
    kb_thread.join();
    proc_thread.join();
    
    return 0;
}