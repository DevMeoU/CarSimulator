#include "../system/linux/keyboard/Keyboard.hpp"
#include "../system/linux/queue/SynchronizedQueue.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include <thread>
#include <atomic>
#include <ncurses.h>

using json = nlohmann::json;

std::atomic<bool> running{true};
system::linux::queue::SynchronizedQueue<json> data_queue(10);

void keyboard_thread() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    
    while (running) {
        json key_data;
        int ch = getch();
        
        if (ch == ' ') key_data["gas"] = 1;
        if (ch == '\n') key_data["brake"] = 1;
        if (ch == 'w') key_data["forward"] = 1;
        if (ch == 's') key_data["backward"] = 1;
        if (ch == 'a') key_data["left"] = 1;
        if (ch == 'd') key_data["right"] = 1;
        
        if (!key_data.empty()) {
            data_queue.enqueue(key_data);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    endwin();
}

void process_thread() {
    while (running) {
        try {
            json data = data_queue.dequeue();
            std::cout << "Key data: " << data.dump(4) << std::endl;
        } catch (...) {
            // Xử lý lỗi
        }
    }
}

int main() {
    std::thread kb_thread(keyboard_thread);
    std::thread proc_thread(process_thread);
    
    // Chạy test trong 5 giây
    std::this_thread::sleep_for(std::chrono::seconds(5));
    
    running = false;
    kb_thread.join();
    proc_thread.join();
    
    return 0;
}