// 平台层：agent 里所有和操作系统相关的东西都从这里创建
//
// 目录结构：
//   agent/capture、input、cursor、clipboard、display、audio   接口（各平台通用）
//   agent/encode、flow、session                               通用实现
//   agent/platform/linux/     X11 / Wayland / uinput / PulseAudio
//   agent/platform/windows/   DXGI / SendInput / WASAPI
//   agent/platform/macos/     ScreenCaptureKit / CGEvent / CoreAudio
// CMake 只编译当前平台的那个目录，每个目录里实现一个 create_platform()。
//
// 为什么用一个 Platform 对象，而不是一组自由函数：
//   有些后端的多个部件要共享状态。比如 Wayland 下截屏和注入输入都要用同一个
//   portal 会话（见 platform/linux/portal_session.h），放在 Platform 对象里最自然。
//
// 参考：
//   Sunshine/src/platform/common.h   Sunshine 也是这种做法：一份接口，linux/windows/macos 各一份实现

#pragma once

#include <memory>

#include "audio/audio_capture.h"
#include "capture/capturer.h"
#include "clipboard/clipboard.h"
#include "cursor/cursor_monitor.h"
#include "display/display_manager.h"
#include "input/injector.h"

import tether.util;

namespace tether::agent {

struct PlatformOptions {
    const char* display = nullptr;   // Linux X11：nullptr 表示用 $DISPLAY；其他平台忽略
    const char* backend = nullptr;   // Linux："x11" / "wayland"，nullptr 表示按 $XDG_SESSION_TYPE 自动选
};

class Platform {
public:
    virtual ~Platform() = default;

    // 日志里用，比如 "linux-x11"、"windows"
    virtual const char* name() const = 0;

    virtual Result<std::unique_ptr<Capturer>> create_capturer(const CaptureRegion& region) = 0;
    // screen_w / screen_h：绝对坐标设备（uinput）需要知道屏幕范围
    virtual Result<std::unique_ptr<Injector>> create_injector(int screen_w, int screen_h) = 0;
    virtual Result<std::unique_ptr<CursorMonitor>> create_cursor_monitor() = 0;
    virtual Result<std::unique_ptr<Clipboard>> create_clipboard() = 0;
    virtual Result<std::unique_ptr<DisplayManager>> create_display_manager() = 0;
    // 没编译声音支持时返回 Errc::Unsupported，Session 应该关掉声音继续跑
    virtual Result<std::unique_ptr<AudioCapture>> create_audio_capture() = 0;
};

// 每个平台目录里实现一份
Result<std::unique_ptr<Platform>> create_platform(const PlatformOptions& opt);

}  // namespace tether::agent
