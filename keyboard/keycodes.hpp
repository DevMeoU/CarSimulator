#ifndef BASE_KEYBOARD_KEYCODES_HPP
#define BASE_KEYBOARD_KEYCODES_HPP

namespace base::keyboard {

/*
 * Key code definitions for both Windows and Linux
 * Windows: Uses Virtual-Key Codes from Windows.h
 * Linux: Uses ncurses key codes
 */

#ifdef _WIN32
#include <Windows.h>

// Basic control keys
static constexpr int KEY_ESCAPE = VK_ESCAPE;
#define KEY_ENTER       VK_RETURN
#define KEY_SPACE       VK_SPACE
#define KEY_TAB         VK_TAB
#define KEY_BACKSPACE   VK_BACK

// Modifier keys
#define KEY_SHIFT       VK_SHIFT
#define KEY_CONTROL     VK_CONTROL
#define KEY_ALT         VK_MENU
#define KEY_CAPS_LOCK   VK_CAPITAL

// Navigation keys
#define KEY_UP          VK_UP
#define KEY_DOWN        VK_DOWN
#define KEY_LEFT        VK_LEFT
#define KEY_RIGHT       VK_RIGHT
#define KEY_PAGE_UP     VK_PRIOR
#define KEY_PAGE_DOWN   VK_NEXT
#define KEY_HOME        VK_HOME
#define KEY_END         VK_END
#define KEY_INSERT      VK_INSERT
#define KEY_DELETE      VK_DELETE

// Function keys
#define KEY_F1          VK_F1
#define KEY_F2          VK_F2
#define KEY_F3          VK_F3
#define KEY_F4          VK_F4
#define KEY_F5          VK_F5
#define KEY_F6          VK_F6
#define KEY_F7          VK_F7
#define KEY_F8          VK_F8
#define KEY_F9          VK_F9
#define KEY_F10         VK_F10
#define KEY_F11         VK_F11
#define KEY_F12         VK_F12

// Phím chuột
#define KEY_MOUSE_LEFT  VK_LBUTTON
#define KEY_MOUSE_RIGHT VK_RBUTTON
#define KEY_MOUSE_MID   VK_MBUTTON

#else // Linux
// Basic control keys
#define KEY_ESCAPE      27
#define KEY_ENTER       '\n'
#define KEY_SPACE       ' '
#define KEY_TAB         '\t'
#define KEY_BACKSPACE   127

// Modifier keys
#define KEY_SHIFT       0x200
#define KEY_CONTROL     KEY_CTRLL
#define KEY_ALT         KEY_ALT_L
// KEY_CAPS_LOCK không có trong ncurses

// Navigation keys đã được định nghĩa trong ncurses:
// KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
// KEY_PPAGE (Page Up), KEY_NPAGE (Page Down)
// KEY_HOME, KEY_END
// KEY_IC (Insert), KEY_DC (Delete)

// Phím chuột
#define KEY_MOUSE_LEFT  BUTTON1_PRESSED
#define KEY_MOUSE_RIGHT BUTTON3_PRESSED
#define KEY_MOUSE_MID   BUTTON2_PRESSED

#endif

// Phím chữ cái (giống nhau trên cả hai nền tảng)
#define KEY_A 'A'
#define KEY_B 'B'
#define KEY_C 'C'
#define KEY_D 'D'
#define KEY_E 'E'
#define KEY_F 'F'
#define KEY_G 'G'
#define KEY_H 'H'
#define KEY_I 'I'
#define KEY_J 'J'
#define KEY_K 'K'
#define KEY_L 'L'
#define KEY_M 'M'
#define KEY_N 'N'
#define KEY_O 'O'
#define KEY_P 'P'
#define KEY_Q 'Q'
#define KEY_R 'R'
#define KEY_S 'S'
#define KEY_T 'T'
#define KEY_U 'U'
#define KEY_V 'V'
#define KEY_W 'W'
#define KEY_X 'X'
#define KEY_Y 'Y'
#define KEY_Z 'Z'

// Phím số (giống nhau trên cả hai nền tảng)
#define KEY_0 '0'
#define KEY_1 '1'
#define KEY_2 '2'
#define KEY_3 '3'
#define KEY_4 '4'
#define KEY_5 '5'
#define KEY_6 '6'
#define KEY_7 '7'
#define KEY_8 '8'
#define KEY_9 '9'

} // namespace base::keyboard

#endif // BASE_KEYBOARD_KEYCODES_HPP