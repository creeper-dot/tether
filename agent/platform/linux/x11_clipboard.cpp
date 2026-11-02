// X11 剪贴板
//
// X11 剪贴板不是"一块共享内存"，而是一套进程间协议：谁拥有 selection，
// 别人要内容时就向它要。所以 agent 要创建一个不可见窗口来当 selection 的拥有者。
// X11 有两个常用 selection：CLIPBOARD（Ctrl+C/V）和 PRIMARY（鼠标选中、中键粘贴）。
//
// 参考：
//   tigervnc/unix/x0vncserver/XSelection.cxx（209 行，完整的 X11 剪贴板收发）
//   tigervnc/unix/x0vncserver/XDesktop.cxx 第 191-194 行：订阅剪贴板变化
//
// 要点：
//   - X11 target 用 UTF8_STRING
//   - Wayland 下需要别的方案（wl-clipboard 或 portal 的 Clipboard 接口），先不做

#include "platform/linux/backends.h"

namespace tether::agent::linux_backend {

namespace {

class X11Clipboard final : public Clipboard {
public:
    explicit X11Clipboard(const char*) {
        // TODO
    }

    bool poll(std::string&) override {
        // TODO
        return false;
    }

    void set(const std::string&) override {
        // TODO
    }

private:
    // TODO: Display*、隐藏窗口、上次设置的内容（防回环）
};

}  // namespace

Result<std::unique_ptr<Clipboard>> create_x11_clipboard(const char* display) {
    return std::make_unique<X11Clipboard>(display);
}

}  // namespace tether::agent::linux_backend
