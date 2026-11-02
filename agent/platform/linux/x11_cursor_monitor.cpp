// X11 光标监视（XFixes）
//
// 参考：
//   tigervnc/unix/x0vncserver/XDesktop.cxx
//     第 187 行附近：XFixesSelectCursorInput 订阅光标形状变化
//     第 892 行附近：处理 XFixesCursorNotify 事件
//     第 1028 行附近：XFixesGetCursorImage 取光标图像
//   FreeRDP/server/shadow/X11/x11_shadow.c   搜 XFixesGetCursorImage
//
// 要点：
//   - XFixesGetCursorImage 返回的像素是 unsigned long 数组（64 位系统上每个 8 字节），
//     不是 uint32_t，转换时要逐个取低 32 位，这是个常见的坑
//   - 位置：形状变化有事件通知，位置没有，需要定时 XQueryPointer
//
// 依赖：libxfixes-dev

#include "platform/linux/backends.h"

namespace tether::agent::linux_backend {

namespace {

class X11CursorMonitor final : public CursorMonitor {
public:
    explicit X11CursorMonitor(const char*) {
        // TODO: XOpenDisplay，XFixesSelectCursorInput
    }

    bool poll_shape(proto::CursorShape&) override {
        // TODO: 处理 XFixesCursorNotify 事件，XFixesGetCursorImage
        return false;
    }

    bool poll_position(proto::CursorPos&) override {
        // TODO: XQueryPointer，和上次比较
        return false;
    }

private:
    // TODO: Display*、xfixes event base、上次位置
};

}  // namespace

Result<std::unique_ptr<CursorMonitor>> create_x11_cursor_monitor(const char* display) {
    return std::make_unique<X11CursorMonitor>(display);
}

}  // namespace tether::agent::linux_backend
