// Windows 光标监视
//
// 两种做法：
//   A. GetCursorInfo（这里用这种，和截屏解耦，简单）
//      - GetCursorInfo(&ci)：ci.hCursor 是当前光标句柄，ci.ptScreenPos 是位置，
//        ci.flags & CURSOR_SHOWING 表示是否显示
//      - 形状变化：比较 hCursor 和上次是否相同，不同才去取图像
//      - 取图像：GetIconInfo(hCursor) 拿到 hbmColor / hbmMask 两张位图 + 热点 xHotspot/yHotspot
//          彩色光标：hbmColor 是 32 位 BGRA（GetDIBits 读出来），可能没有 alpha（全 0），
//                   这时要用 hbmMask 补 alpha
//          黑白光标：hbmColor == nullptr，hbmMask 高度是光标的两倍，上半是 AND 掩码，下半是 XOR 掩码
//                   AND=0 XOR=0 黑，AND=0 XOR=1 白，AND=1 XOR=0 透明，AND=1 XOR=1 反色（文本光标 I 型）
//                   反色没法用 RGBA 表示，一般画成黑色或者带白边的黑色
//        用完要 DeleteObject(hbmColor)、DeleteObject(hbmMask)，否则 GDI 句柄泄漏
//      - 协议要的是预乘 alpha 的 RGBA，Windows 给的是 BGRA，要交换 R/B 并预乘
//
//   B. 从 DXGI 帧里拿：frame_info.PointerShapeBufferSize > 0 时 GetFramePointerShape
//      和截屏绑在一起，光标位置精确同步。Sunshine 用这种（把光标画进画面）
//
// 坐标：ptScreenPos 是虚拟桌面坐标，和协议一致（前提是进程 DPI Aware，见 backends.h）
//
// 参考：
//   Sunshine/src/platform/windows/display_vram.cpp
//     make_cursor_xor_image() / make_cursor_alpha_image()（第 280、356 行附近）
//     三种 DXGI_OUTDUPL_POINTER_SHAPE_TYPE（彩色 / 黑白 / 带掩码彩色）怎么转成 RGBA
//   Sunshine/src/platform/windows/display_ram.cpp   blend_cursor_*()，同样三种格式
//   tigervnc/win/rfb_win32/DeviceFrameBuffer.cxx   setCursor()：HCURSOR -> 像素 + 掩码
//   tigervnc/win/rfb_win32/IconInfo.h              GetIconInfo 的 RAII 包装（自动 DeleteObject）
//   learn.microsoft.com "GetCursorInfo"、"GetIconInfo"、"GetDIBits"

#include "platform/windows/backends.h"
#include "platform/windows/win_util.h"

namespace tether::agent::windows_backend {

namespace {

class WinCursorMonitor final : public CursorMonitor {
public:
    bool poll_shape(proto::CursorShape&) override {
        // TODO: GetCursorInfo，hCursor 变了 -> GetIconInfo -> GetDIBits -> 转 RGBA 预乘
        return false;
    }

    bool poll_position(proto::CursorPos&) override {
        // TODO: GetCursorInfo，位置或可见性变了返回 true
        return false;
    }

private:
    // TODO: 上次的 HCURSOR（用 void* 存，或者 include win_util.h 后直接用 HCURSOR）、上次位置
};

}  // namespace

Result<std::unique_ptr<CursorMonitor>> create_win_cursor_monitor() {
    return std::make_unique<WinCursorMonitor>();
}

}  // namespace tether::agent::windows_backend
