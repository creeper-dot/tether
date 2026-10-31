// SDL 键码 -> 协议里的 keysym + scancode
//
// SDL 给每个按键事件两个值：
//   SDL_Keycode  这个键代表什么（受键盘布局影响）-> 转成 X11 keysym
//   SDL_Scancode 物理位置（USB HID 编码）        -> 转成 evdev 键码
//
// 参考：
//   moonlight-qt/app/streaming/input/keyboard.cpp   SDL_Scancode -> Windows VK 码的大表，结构可以照抄
//   tigervnc/vncviewer/KeyboardX11.cxx              X11 下怎么取 keysym
//   tigervnc/vncviewer/keysym2ucs.c                 Unicode <-> keysym 转换
//   SDL2 源码 src/events/scancodes_linux.h          evdev -> SDL_Scancode 的现成表，反过来用就行
//   /usr/include/X11/keysymdef.h                    keysym 定义
//
// 要点：
//   - 字母、数字、标点：keysym 等于对应的 ASCII/Latin-1 码，可以直接用
//   - 功能键、方向键、小键盘、修饰键要逐个映射
//   - 输入法（中文输入）是另一回事：SDL_TEXTINPUT 事件给的是 UTF-8 文本，
//     要转成 keysym（Unicode 字符 c 对应 keysym 0x01000000 | c）

#pragma once

#include <cstdint>

namespace tether::client {

std::uint32_t sdl_keycode_to_keysym(std::int32_t sdl_keycode);
std::uint32_t sdl_scancode_to_evdev(int sdl_scancode);

}  // namespace tether::client
