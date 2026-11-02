// 键码转换（agent 侧，macOS）
//
// macOS 的键码是 CGKeyCode（kVK_* 常量，定义在 <Carbon/Carbon.h> 的 HIToolbox/Events.h），
// 表示物理位置（按 ANSI 美式键盘命名，比如 kVK_ANSI_A = 0x00），和 evdev 一样是"位置码"，
// 所以优先用协议里的 scancode（evdev）查表转换，和键盘布局无关。
//
// 没有 scancode 时（client 只给了 keysym）：
//   - 功能键、方向键、修饰键：keysym -> kVK 查表
//   - 普通字符：要按当前键盘布局反查（TISCopyCurrentKeyboardLayoutInputSource + UCKeyTranslate），
//     比较麻烦，先不做；或者直接用 CGEventKeyboardSetUnicodeString 发字符
//
// 修饰键注意：
//   macOS 的 Command 键对应 evdev 的 KEY_LEFTMETA / KEY_RIGHTMETA（Windows 键 / Super 键），
//   Option 对应 Alt。client 是 Windows/Linux 时，用户按 Ctrl+C 想复制，
//   在 macOS 上其实应该是 Command+C，要不要交换 Ctrl 和 Command 可以做成一个选项
//
// 参考：
//   Sunshine v0.23.1 src/platform/macos/input.cpp   kKeyCodesMap（第 50 行附近）：Windows VK -> kVK 的表
//   /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/System/Library/Frameworks/
//     Carbon.framework/Frameworks/HIToolbox.framework/Headers/Events.h   kVK_* 定义

#pragma once

#include <cstdint>

namespace tether::agent::macos_backend {

// 找不到返回 0xFFFF（kVK_ANSI_A 就是 0，所以不能用 0 表示没找到）
inline constexpr std::uint16_t kNoKeyCode = 0xFFFF;

std::uint16_t evdev_to_cgkeycode(std::uint32_t evdev);
std::uint16_t keysym_to_cgkeycode(std::uint32_t keysym);

// 修饰键对应的 CGEventFlags 位（kCGEventFlagMaskShift 等），不是修饰键返回 0
std::uint64_t modifier_flag_for_cgkeycode(std::uint16_t keycode);

}  // namespace tether::agent::macos_backend
