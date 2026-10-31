// 截屏接口
//
// 实现：
//   x11_capture       X11（MVP）
//   pipewire_capture  Wayland，通过 xdg-desktop-portal 拿 PipeWire 流
//   以后可选：kms_capture（直接读 DRM，需要 root/CAP_SYS_ADMIN，可以截登录界面）
//
// 参考：
//   Sunshine/src/platform/common.h   display_t 接口：snapshot()、分辨率变化、硬件帧
//   FreeRDP/server/shadow/shadow_subsystem.h   另一种"截屏后端"接口设计，可以对比

#pragma once

#include <cstdint>
#include <memory>
#include <vector>

import tether.util;

namespace tether::agent {

// 一帧原始画面，像素格式 BGRA，每像素 4 字节
struct RawFrame {
    int width = 0;
    int height = 0;
    int stride = 0;                  // 每行字节数，可能 > width*4（行尾有对齐填充）
    std::uint64_t timestamp_us = 0;
    std::vector<std::uint8_t> pixels;  // 大小 = stride * height
};

enum class GrabStatus {
    NewFrame,   // 拿到新画面，已写进 out
    NoChange,   // 画面没变（用 XDamage 或 PipeWire 判断），可以跳过编码省带宽
    Reinit,     // 分辨率或显示器变了，调用方应重建 Capturer 和 Encoder
};

class Capturer {
public:
    virtual ~Capturer() = default;

    // 截一帧。timeout_ms 内画面没变就返回 NoChange；真正出错返回 Error
    virtual Result<GrabStatus> grab(RawFrame& out, int timeout_ms) = 0;

    virtual int width() const = 0;
    virtual int height() const = 0;
};

// x/y/w/h 指定截取区域（多显示器时选其中一个）；w==0 表示整个屏幕
struct CaptureRegion {
    int x = 0, y = 0, w = 0, h = 0;
};

// display 为 nullptr 时用环境变量 DISPLAY
Result<std::unique_ptr<Capturer>> create_x11_capturer(const char* display,
                                                      const CaptureRegion& region);

#ifdef TETHER_HAVE_PIPEWIRE
class PortalSession;
// 需要先建好 portal 会话（见 portal/portal_session.h）
Result<std::unique_ptr<Capturer>> create_pipewire_capturer(PortalSession& portal);
#endif

}  // namespace tether::agent
