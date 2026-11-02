// macOS 各后端的创建函数，只给 macos_platform.cpp 用
//
// 坐标约定（重要）：
//   macOS 的 CG 全局坐标单位是"点"（point），不是像素。Retina 屏上 1 点 = 2 像素。
//     CGDisplayBounds、CGEventGetLocation、CGEventCreateMouseEvent  都用点
//     ScreenCaptureKit 截出来的画面、CGDisplayModeGetPixelWidth      都是像素
//   协议里统一用像素（client 看到的就是截屏画面），所以：
//     注入：像素 -> 点，除以缩放比例
//     光标位置、显示器布局：点 -> 像素，乘以缩放比例
//   缩放比例 = CGDisplayModeGetPixelWidth(mode) / CGDisplayModeGetWidth(mode)，每个显示器可能不同
//   原点：主屏左上角是 (0,0)，y 向下（CG 坐标；AppKit 的 NSScreen 是左下角原点 y 向上，别混用）

#pragma once

#include <cstdint>
#include <expected>
#include <memory>

#include "audio/audio_capture.h"
#include "capture/capturer.h"
#include "clipboard/clipboard.h"
#include "cursor/cursor_monitor.h"
#include "display/display_manager.h"
#include "input/injector.h"

import tether.util;

namespace tether::agent::macos_backend {

Result<std::unique_ptr<Capturer>> create_sck_capturer(const CaptureRegion& region);
Result<std::unique_ptr<Injector>> create_cgevent_injector();
Result<std::unique_ptr<CursorMonitor>> create_mac_cursor_monitor();
Result<std::unique_ptr<Clipboard>> create_mac_clipboard();
Result<std::unique_ptr<DisplayManager>> create_mac_display();
Result<std::unique_ptr<AudioCapture>> create_sck_audio_capture();

// 像素 <-> 点 的缩放比例（主屏），实现在 mac_display.cpp
// 多显示器且缩放不同时要按坐标所在的显示器取，先只做主屏
double main_display_scale();

}  // namespace tether::agent::macos_backend
