#include "../../base/producer_consumer/ProducerConsumer.hpp"
#include "../../base/queue/SynchronizedQueue.hpp"
#include "../../keyboard/keycodes.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <fstream>
#include <mutex>
#include <ctime>
#include <memory>
#include <cstddef>

#ifdef _WIN32
#include <Windows.h>
#else
#include <ncurses.h>
#endif

std::atomic<bool> running{true};
std::mutex logMutex;
std::unique_ptr<std::ofstream> logFile;

struct KeyState {
    bool isPressed = false;
    std::chrono::steady_clock::time_point lastPressTime;
};

class KeyboardProducer : public base::producer_consumer::IProducer<int> {
public:
    KeyboardProducer() {
        logFile = std::make_unique<std::ofstream>("keylog.txt", std::ios::app);
        if (!logFile->is_open()) {
            std::cerr << "Failed to open keylog.txt" << std::endl;
        }
    }
    
    bool isRunning() const override { return running; }
    void stop() override { running = false; }
    void produce(base::queue::SynchronizedQueue<int>& queue) override;

private:
    void logKeyPress(int key);
    std::array<KeyState, 256> keyStates;
    const int KEY_REPEAT_DELAY_MS = 50;
};

class KeyboardConsumer : public base::producer_consumer::IConsumer<int> {
public:
    bool isRunning() const override { return running; }
    void stop() override { running = false; }
    void consume(base::queue::SynchronizedQueue<int>& queue) override;
};

void KeyboardProducer::logKeyPress(int key) {
    if (!logFile || !logFile->is_open()) return;

    std::lock_guard<std::mutex> lock(logMutex);
    auto timeNow = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(timeNow);
    char timestamp[26];
    std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&time));

    #ifdef _WIN32
    UINT scanCode = MapVirtualKeyA(key, MAPVK_VK_TO_VSC);
    char keyName[32] = "Unknown";
    if (scanCode) {
        if (GetKeyNameTextA(scanCode << 16, keyName, sizeof(keyName)) == 0) {
            strcpy(keyName, "Unknown");
        }
    }
    *logFile << "[" << timestamp << "] Key pressed: " << keyName << std::endl;
    #else
    std::string keyName;
    if (key >= base::keyboard::KEY_MIN && key <= base::keyboard::KEY_MAX) {
        keyName = keyname(key);
    } else {
        keyName = std::string(1, (char)key);
    }
    *logFile << "[" << timestamp << "] Key pressed: " << keyName << std::endl;
    #endif
}

void KeyboardProducer::produce(base::queue::SynchronizedQueue<int>& queue) {
    #ifdef _WIN32
    for (int key = 0; key < 256; ++key) {
        bool currentState = (GetAsyncKeyState(key) & 0x8000) != 0;
        auto now = std::chrono::steady_clock::now();

        if (currentState) {
            if (!keyStates[key].isPressed || 
                std::chrono::duration_cast<std::chrono::milliseconds>
                (now - keyStates[key].lastPressTime).count() >= KEY_REPEAT_DELAY_MS) {
                {
                    std::lock_guard<std::mutex> lock(logMutex);
                    queue.enqueue(key);
                    logKeyPress(key);
                }
                keyStates[key].lastPressTime = now;
            }
        }
        keyStates[key].isPressed = currentState;

        if (key == base::keyboard::KEY_ESCAPE && currentState) {
            running = false;
            break;
        }
    }
    #else
    timeout(100);
    int ch = getch();
    if (ch != ERR) {
        auto now = std::chrono::steady_clock::now();
        if (!keyStates[ch & 0xFF].isPressed || 
            std::chrono::duration_cast<std::chrono::milliseconds>
            (now - keyStates[ch & 0xFF].lastPressTime).count() >= KEY_REPEAT_DELAY_MS) {
            {
                std::lock_guard<std::mutex> lock(logMutex);
                queue.enqueue(ch);
                logKeyPress(ch);
            }
            keyStates[ch & 0xFF].lastPressTime = now;
        }
        keyStates[ch & 0xFF].isPressed = true;

        if (ch == base::keyboard::KEY_ESCAPE) {
            running = false;
        }
    } else {
        for (auto& state : keyStates) {
            state.isPressed = false;
        }
    }
    #endif
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void KeyboardConsumer::consume(base::queue::SynchronizedQueue<int>& queue) {
    int key = queue.dequeue();
    std::cout << "Key pressed: " << key;
    #ifdef _WIN32
    UINT scanCode = ::MapVirtualKeyA(key, MAPVK_VK_TO_VSC);
    char keyName[32] = "Unknown";
    if (scanCode) {
        ::GetKeyNameTextA(scanCode << 16, keyName, sizeof(keyName));
        std::cout << " (" << keyName << ")";
    }
    #else
    if (key >= base::keyboard::KEY_MIN && key <= base::keyboard::KEY_MAX) {
        std::cout << " (" << keyname(key) << ")";
    }
    #endif
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

int main() {
    base::queue::SynchronizedQueue<int> queue(100);
    KeyboardProducer producer;
    KeyboardConsumer consumer;

    #ifdef __linux__
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    nodelay(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    #endif

    std::thread producerThread([&]() {
        while (producer.isRunning()) {
            producer.produce(queue);
        }
    });

    std::thread consumerThread([&]() {
        while (consumer.isRunning()) {
            consumer.consume(queue);
        }
    });

    std::cout << "Press ESC to exit..." << std::endl;

    producerThread.join();
    consumerThread.join();

    #ifdef __linux__
    endwin();
    #endif

    return 0;
}