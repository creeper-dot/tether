// Windows 各后端的创建函数，只给 windows_platform.cpp 用
//
// 这个头文件不 include <windows.h>，Windows 类型只出现在各 .cpp 和 win_util.h 里。
//
// 坐标约定：
//   协议里的坐标都是"虚拟桌面像素坐标"（多显示器拼起来的大桌面，主屏左上角是 (0,0)，
//   左边/上边的副屏坐标可以是负数）。截屏、光标、注入、显示器布局都用这一套。
//   要拿到真实像素坐标，进程必须是 Per-Monitor DPI Aware（见 windows_platform.cpp），
//   否则系统会按 DPI 缩放给你一套"假坐标"，缩放 150% 时鼠标会点歪。

#pragma once

#include <expected>
#include <memory>

#include "audio/audio_capture.h"
#include "capture/capturer.h"
#include "clipboard/clipboard.h"
#include "cursor/cursor_monitor.h"
#include "display/display_manager.h"
#include "input/injector.h"

import tether.util;

namespace tether::agent::windows_backend {

Result<std::unique_ptr<Capturer>> create_dxgi_capturer(const CaptureRegion& region);
Result<std::unique_ptr<Injector>> create_sendinput_injector();
Result<std::unique_ptr<CursorMonitor>> create_win_cursor_monitor();
Result<std::unique_ptr<Clipboard>> create_win_clipboard();
Result<std::unique_ptr<DisplayManager>> create_win_display();
Result<std::unique_ptr<AudioCapture>> create_wasapi_audio_capture();

}  // namespace tether::agent::windows_backend
