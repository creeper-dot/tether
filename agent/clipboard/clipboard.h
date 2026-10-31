// 剪贴板同步（agent 侧，X11）
//
// X11 剪贴板不是"一块共享内存"，而是一套进程间协议：谁拥有 selection，
// 别人要内容时就向它要。所以 agent 要创建一个不可见窗口来当 selection 的拥有者。
// X11 有两个常用 selection：CLIPBOARD（Ctrl+C/V）和 PRIMARY（鼠标选中、中键粘贴）。
//
// 参考：
//   tigervnc/unix/x0vncserver/XSelection.cxx（209 行，完整的 X11 剪贴板收发）
//   tigervnc/unix/x0vncserver/XDesktop.cxx 第 191-194 行：订阅剪贴板变化
//   tigervnc/common/rfb/clipboardTypes.h   剪贴板格式定义
//   FreeRDP/channels/cliprdr               RDP 剪贴板通道，看它怎么设计"先通知、再按需取"
//
// 要点：
//   - 文本统一用 UTF-8（X11 target 是 UTF8_STRING）
//   - 防回环：client 发来的内容设置到远端后，远端会通知"剪贴板变了"，
//     这时不要再发回 client。记住自己刚设置的内容，相同就跳过
//   - 大小限制，比如 1MB，超过就不同步
//   - Wayland 下需要别的方案（wl-clipboard 或 portal 的 Clipboard 接口），先不做

#pragma once

#include <string>

namespace tether::agent {

class Clipboard {
public:
    explicit Clipboard(const char* display);
    ~Clipboard();

    // 远端剪贴板内容变了返回 true，内容写进 text
    bool poll(std::string& text);

    // client 发来新内容，设置到远端剪贴板
    void set(const std::string& text);

private:
    // TODO: Display*、隐藏窗口、上次设置的内容（防回环）
};

}  // namespace tether::agent
