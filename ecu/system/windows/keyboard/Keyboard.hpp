#ifndef SYSTEM_WINDOWS_KEYBOARD_HPP
#define SYSTEM_WINDOWS_KEYBOARD_HPP

#include <Windows.h>
#include <array>
#include <chrono>

namespace system {
namespace windows {
namespace keyboard {

struct KeyState {
    bool isPressed = false;
    std::chrono::steady_clock::time_point lastPressTime;
};

constexpr int KEY_ESCAPE = VK_ESCAPE;
constexpr int KEY_MIN = 0;
constexpr int KEY_MAX = 255;

static const char* getKeyName(int keyCode) {
    static char keyName[32] = "Unknown";
    UINT scanCode = MapVirtualKeyA(keyCode, MAPVK_VK_TO_VSC);
    if (scanCode) {
        GetKeyNameTextA(scanCode << 16, keyName, sizeof(keyName));
    }
    return keyName;
}

} // namespace keyboard
} // namespace windows
} // namespace system

#endif // SYSTEM_WINDOWS_KEYBOARD_HPP