// 键码转换（agent 侧）
//
// 协议里每个按键同时带 keysym（这个键代表什么）和 scancode（物理位置，evdev 编码）。
// 不同注入方式需要的东西不一样：
//   XTest   要 X11 keycode：优先用 scancode + 8（evdev 键码 +8 就是 X11 keycode），
//           没有 scancode 时用 XKeysymToKeycode(keysym)
//   uinput  要 evdev 键码：直接用 scancode
//   portal  要 evdev 键码：同上
//
// 难点：远端键盘布局里找不到某个 keysym（比如远端是美式布局，client 发来一个 é）。
// TigerVNC 的做法是临时改远端的键盘映射，把这个 keysym 绑到一个空闲的 keycode 上。
//
// 参考：
//   tigervnc/unix/x0vncserver/XDesktop.cxx
//     第 616 行之前：keysym -> keycode 的查找逻辑，包括找不到时的处理
//   tigervnc/unix/x0vncserver/qnum_to_xorgevdev.c   扫描码 -> evdev 的对照表
//   /usr/include/linux/input-event-codes.h  evdev 键码定义

#pragma once

#include <cstdint>

namespace tether::agent {

// evdev 键码 -> X11 keycode（就是 +8，单独写成函数是为了让代码意图清楚）
inline std::uint32_t evdev_to_x11_keycode(std::uint32_t evdev) { return evdev + 8; }

// 修饰键（Shift、Ctrl、Alt、Super）的 keysym -> evdev 键码；不是修饰键返回 0
std::uint32_t modifier_keysym_to_evdev(std::uint32_t keysym);

}  // namespace tether::agent
