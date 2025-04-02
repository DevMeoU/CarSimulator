#include "KeyboardThread.h"
#include "VehicleData.h"
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

KeyboardThread::KeyboardThread(std::shared_ptr<VehicleData> data)
    : running(false), vehicleData(data) {}

void KeyboardThread::start() {
    running = true;
    thread = std::thread(&KeyboardThread::threadFunction, this);
}

void KeyboardThread::stop() {
    running = false;
    if (thread.joinable()) {
        thread.join();
    }
}

void KeyboardThread::threadFunction() {
    while (running) {
        int key = getKeyPress();
        if (key != 0) {
            processKeyEvent(key);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

int KeyboardThread::getKeyPress() {
#ifdef _WIN32
    if (_kbhit()) {
        return _getch();
    }
#else
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    return (ch != EOF) ? ch : 0;
#endif
    return 0;
}

void KeyboardThread::processKeyEvent(int key) {
    std::lock_guard<std::mutex> lock(vehicleData->mutex);

    switch (key) {
        case ' ': // SPACE - Tăng tốc
            vehicleData->gas = true;
            vehicleData->brake = false;
            break;
        case 'b': // B - Phanh
            vehicleData->brake = true;
            vehicleData->gas = false;
            break;
        case 'p': // P - Phanh tay
            vehicleData->park = !vehicleData->park;
            break;
        case 'l': // L - Khóa/mở cửa
            vehicleData->door_lock = !vehicleData->door_lock;
            break;
        case 's': // S - Dây an toàn
            vehicleData->seat_belt = !vehicleData->seat_belt;
            break;
        case 75: // LEFT ARROW - Xi-nhan trái
            vehicleData->signal_left = !vehicleData->signal_left;
            break;
        case 77: // RIGHT ARROW - Xi-nhan phải
            vehicleData->signal_right = !vehicleData->signal_right;
            break;
        case '1': // Chế độ Normal
            vehicleData->mode = "Normal";
            break;
        case '2': // Chế độ Sport
            vehicleData->mode = "Sport";
            break;
        case '3': // Chế độ Eco
            vehicleData->mode = "Eco";
            break;
        case '4': // Chế độ Snow/Off-road
            vehicleData->mode = "Snow/Off-road";
            break;
        case 27: // ESC - Thoát
            vehicleData->running = false;
            break;
    }
}