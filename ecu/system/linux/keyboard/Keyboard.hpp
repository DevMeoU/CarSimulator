#ifndef SYSTEM_LINUX_KEYBOARD_HPP
#define SYSTEM_LINUX_KEYBOARD_HPP

#include <ncurses.h>
#include <array>
#include <chrono>

namespace system {
namespace linux {
namespace keyboard {

struct KeyState {
    bool isPressed = false;
    std::chrono::steady_clock::time_point lastPressTime;
};

constexpr int KEY_ESCAPE = 27;
constexpr int KEY_MIN = 0;
constexpr int KEY_MAX = 255;

static const char* getKeyName(int keyCode) {
    if (keyCode >= KEY_MIN && keyCode <= KEY_MAX) {
        return keyname(keyCode);
    }
    return "Unknown";
}

} // namespace keyboard
} // namespace linux
} // namespace system

#endif // SYSTEM_LINUX_KEYBOARD_HPP