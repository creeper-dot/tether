// 剪贴板同步接口（agent 侧，各平台通用）
//
// 实现：
//   linux    x11_clipboard（Wayland 需要 wl-clipboard 或 portal 的 Clipboard 接口，先不做）
//   windows  win_clipboard（OpenClipboard / AddClipboardFormatListener）
//   macos    mac_clipboard（NSPasteboard，轮询 changeCount）
//
// 通用要点：
//   - 协议里的文本统一用 UTF-8；Windows 剪贴板是 UTF-16（CF_UNICODETEXT），要转换
//   - 防回环：client 发来的内容设置到远端后，远端会通知"剪贴板变了"，
//     这时不要再发回 client。记住自己刚设置的内容，相同就跳过
//   - 大小限制，比如 1MB，超过就不同步
//
// 参考：
//   tigervnc/common/rfb/clipboardTypes.h   剪贴板格式定义
//   FreeRDP/channels/cliprdr               RDP 剪贴板通道，看它怎么设计"先通知、再按需取"

#pragma once

#include <string>

namespace tether::agent {

class Clipboard {
public:
    virtual ~Clipboard() = default;

    // 远端剪贴板内容变了返回 true，内容写进 text
    virtual bool poll(std::string& text) = 0;

    // client 发来新内容，设置到远端剪贴板
    virtual void set(const std::string& text) = 0;
};

}  // namespace tether::agent
