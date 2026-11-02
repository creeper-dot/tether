// Linux 各后端的创建函数，只给 linux_platform.cpp 用
//
// 选择逻辑在 linux_platform.cpp：
//   X11      x11_capture + xtest_injector（或 uinput_injector）
//   Wayland  portal_session + pipewire_capture + portal_injector

#pragma once

#include <memory>

#include "audio/audio_capture.h"
#include "capture/capturer.h"
#include "clipboard/clipboard.h"
#include "cursor/cursor_monitor.h"
#include "display/display_manager.h"
#include "input/injector.h"

import tether.util;

namespace tether::agent::linux_backend {

// display 为 nullptr 时用环境变量 DISPLAY
Result<std::unique_ptr<Capturer>> create_x11_capturer(const char* display,
                                                      const CaptureRegion& region);
Result<std::unique_ptr<Injector>> create_xtest_injector(const char* display);
Result<std::unique_ptr<CursorMonitor>> create_x11_cursor_monitor(const char* display);
Result<std::unique_ptr<Clipboard>> create_x11_clipboard(const char* display);
Result<std::unique_ptr<DisplayManager>> create_xrandr_display(const char* display);

#ifdef TETHER_HAVE_UINPUT
// width/height 是屏幕尺寸，绝对坐标设备需要知道范围
Result<std::unique_ptr<Injector>> create_uinput_injector(int width, int height);
#endif

#ifdef TETHER_HAVE_PIPEWIRE
class PortalSession;
// 需要先建好 portal 会话（见 portal_session.h）
Result<std::unique_ptr<Capturer>> create_pipewire_capturer(PortalSession& portal);
Result<std::unique_ptr<Injector>> create_portal_injector(PortalSession& portal);
#endif

#ifdef TETHER_HAVE_AUDIO
Result<std::unique_ptr<AudioCapture>> create_pulse_audio_capture();
#endif

}  // namespace tether::agent::linux_backend
