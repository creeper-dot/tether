// 键码转换（agent 侧，Windows）
//
// 协议里每个按键带两个值（见 client/input/keymap.h）：
//   keysym    X11 keysym，"这个键代表什么"
//   scancode  evdev 键码，"物理上是哪个键"
//
// SendInput 发键盘事件有三种方式：
//   1. 扫描码（KEYEVENTF_SCANCODE）  按物理位置，游戏和快捷键最可靠，优先用这个
//   2. 虚拟键码 VK_*                 没有 scancode 时，用 keysym 查 VK
//   3. Unicode（KEYEVENTF_UNICODE）  keysym 是 0x01000000|c 这种 Unicode 字符（中文输入法发来的）
//                                    时直接发字符，不经过键盘布局
//
// evdev 和 PC 扫描码（Set 1）的关系：
//   KEY_ESC(1) ~ KEY_KPDOT(83) 这一段，evdev 值就等于 Set 1 扫描码，可以直接用
//   方向键、Home/End、右 Ctrl、右 Alt、小键盘回车等是"扩展键"，扫描码前面有 E0，
//   SendInput 里要加 KEYEVENTF_EXTENDEDKEY，evdev 值和扫描码也不相等，要查表
//   比如 KEY_UP(103) -> E0 48，KEY_RIGHTCTRL(97) -> E0 1D
//
// 参考：
//   moonlight-qt/app/streaming/input/keyboard.cpp   SDL_Scancode -> VK 的大表
//   Sunshine v0.23.1 src/platform/windows/input.cpp  keyboard()：VK -> 扫描码、扩展键判断
//   tigervnc/win/rfb_win32/keymap.h                  keysym -> VK 的对照表
//   learn.microsoft.com "Keyboard Input Overview" 里的扫描码表

#pragma once

#include <cstdint>

namespace tether::agent::windows_backend {

struct WinScancode {
    std::uint16_t code = 0;    // Set 1 扫描码（不含 E0 前缀）
    bool extended = false;     // 有 E0 前缀 -> KEYEVENTF_EXTENDEDKEY
};

// 找不到返回 code == 0
WinScancode evdev_to_win_scancode(std::uint32_t evdev);

// 找不到返回 0
std::uint16_t keysym_to_vk(std::uint32_t keysym);

// keysym 是 Unicode 字符时返回 true 并给出 UTF-32 码点（0x01000000 | c，或 Latin-1 范围的 keysym）
bool keysym_to_unicode(std::uint32_t keysym, char32_t& out);

}  // namespace tether::agent::windows_backend
