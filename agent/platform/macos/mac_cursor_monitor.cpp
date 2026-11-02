// macOS 光标监视：CursorMonitor 接口实现
//
// 形状：objc::current_cursor_id() 和上次不同时，objc::current_cursor_image() 取图像
// 位置：CGEventRef e = CGEventCreate(nullptr); CGPoint p = CGEventGetLocation(e); CFRelease(e);
//       单位是点，要乘 main_display_scale() 换成像素
// 可见性：CGCursorIsVisible() 已废弃且不准，一般当作一直可见
//
// 参考：
//   Sunshine v0.23.1 src/platform/macos/input.cpp   get_mouse_loc()（第 319 行附近）
//   见 ns_cursor.mm

#include "platform/macos/backends.h"
#include "platform/macos/objc_bridge.h"

// #include <ApplicationServices/ApplicationServices.h>

namespace tether::agent::macos_backend {

namespace {

class MacCursorMonitor final : public CursorMonitor {
public:
    bool poll_shape(proto::CursorShape&) override {
        // TODO
        return false;
    }

    bool poll_position(proto::CursorPos&) override {
        // TODO
        return false;
    }

private:
    std::uintptr_t last_id_ = 0;
    int last_x_ = -1, last_y_ = -1;
};

}  // namespace

Result<std::unique_ptr<CursorMonitor>> create_mac_cursor_monitor() {
    return std::make_unique<MacCursorMonitor>();
}

}  // namespace tether::agent::macos_backend
