#include "../../base/producer_consumer/ProducerConsumer.hpp"
#include "../../../keyboard/keycodes.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <fstream>
#include <mutex>
#include <ctime>
#include <string>

#ifdef _WIN32
#include <Windows.h>
#else
#include <ncurses.h>
#endif

namespace test {

std::atomic<bool> running{true};
std::mutex logMutex;
std::ofstream logFile;

// Lớp xử lý và ghi log phím bấm
class KeyLogger {
public:
    static void init() {
        logFile.open("keylog.txt", std::ios::app);
    }

    static void log(const std::string& keyName) {
        std::lock_guard<std::mutex> lock(logMutex);
        if(logFile.is_open()) {
            auto now = std::chrono::system_clock::now();
            std::time_t time = std::chrono::system_clock::to_time_t(now);
            char timestamp[26];
            std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&time));
            logFile << "[" << timestamp << "] Key pressed: " << keyName << std::endl;
        }
    }

    static void cleanup() {
        if(logFile.is_open()) {
            logFile.close();
        }
    }
};

// Lớp xử lý input từ bàn phím
class KeyboardProducer : public base::IProducer<int> {
public:
    bool isRunning() const override { return running; }
    void stop() override { running = false; }
    void produce(base::SynchronizedQueue<int>& queue) override;

private:
    bool keyStates[256] = {false};
    std::chrono::steady_clock::time_point lastKeyPress[256];
    const int KEY_REPEAT_DELAY_MS = 50;

    #ifdef _WIN32
    static const char* getKeyName(int keyCode) {
        static char keyName[32];
        if (keyCode >= KEY_MOUSE_LEFT && keyCode <= KEY_MOUSE_RIGHT) {
            const char* mouseButtons[] = {"Mouse Left", "Mouse Right"};
            return mouseButtons[keyCode - KEY_MOUSE_LEFT];
        }
        UINT scanCode = MapVirtualKeyA(keyCode, MAPVK_VK_TO_VSC);
        if(scanCode) {
            GetKeyNameTextA(scanCode << 16, keyName, sizeof(keyName));
            if(keyName[0] != '\0') return keyName;
        }
        switch(keyCode) {
            case VK_SHIFT: return "Shift";
            case VK_CONTROL: return "Control";
            case VK_MENU: return "Alt";
            case VK_RETURN: return "Enter";
            case VK_SPACE: return "Space";
            case VK_TAB: return "Tab";
            case VK_BACK: return "Backspace";
            case VK_ESCAPE: return "Escape";
            default: {
                sprintf_s(keyName, "Key(0x%02X)", keyCode);
                return keyName;
            }
        }
    }
    #endif
};

// Lớp xử lý và hiển thị phím đã bấm
class KeyboardConsumer : public base::IConsumer<int> {
public:
    bool isRunning() const override { return running; }
    void stop() override { running = false; }
    void consume(base::SynchronizedQueue<int>& queue) override;
};

void KeyboardProducer::produce(base::SynchronizedQueue<int>& queue) {
    #ifdef _WIN32
    for(int key = 0; key <= 0xFF; ++key) {
        bool currentState = (GetAsyncKeyState(key) & 0x8000) != 0;
        auto now = std::chrono::steady_clock::now();
        
        if(currentState) {
            if(!keyStates[key] || 
               std::chrono::duration_cast<std::chrono::milliseconds>(now - lastKeyPress[key]).count() >= KEY_REPEAT_DELAY_MS) {
                queue.enqueue(key);
                KeyLogger::log(getKeyName(key));
                lastKeyPress[key] = now;
            }
        }
        keyStates[key] = currentState;
    }
    #else
    timeout(100);
    int ch = getch();
    if(ch != ERR) {
        auto now = std::chrono::steady_clock::now();
        if(!keyStates[ch] || 
           std::chrono::duration_cast<std::chrono::milliseconds>(now - lastKeyPress[ch]).count() >= KEY_REPEAT_DELAY_MS) {
            queue.enqueue(ch);
            std::string keyName;
            if(ch >= KEY_MIN && ch <= KEY_MAX) {
                keyName = keyname(ch);
            } else {
                keyName = std::string(1, (char)ch);
            }
            KeyLogger::log(keyName);
            lastKeyPress[ch] = now;
        }
        keyStates[ch] = true;
        if(ch == 27) { // ESC key
            running = false;
        }
    } else {
        for(int i = 0; i < 256; ++i) keyStates[i] = false;
    }
    #endif
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void KeyboardConsumer::consume(base::SynchronizedQueue<int>& queue) {
    int key = queue.dequeue();
    std::cout << "Key pressed: " << key;
    #ifdef _WIN32
    std::cout << " (" << getKeyName(key) << ")";
    #endif
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

} // namespace test

int main() {
    base::SynchronizedQueue<int> queue(100);
    test::KeyboardProducer producer;
    test::KeyboardConsumer consumer;
    test::KeyLogger::init();

    #ifdef __linux__
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    nodelay(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    #endif

    std::thread producerThread([&producer, &queue]() {
        while(producer.isRunning()) {
            producer.produce(queue);
        }
    });

    std::thread consumerThread([&consumer, &queue]() {
        while(consumer.isRunning()) {
            consumer.consume(queue);
        }
    });

    std::cout << "Press ESC to exit..." << std::endl;
    while(test::running) {
        #ifdef _WIN32
        if(GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            test::running = false;
            break;
        }
        #endif
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    producer.stop();
    consumer.stop();
    producerThread.join();
    consumerThread.join();
    test::KeyLogger::cleanup();

    #ifdef __linux__
    endwin();
    #endif

    return 0;
}