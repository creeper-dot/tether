// 光标监视接口（各平台通用）
//
// 光标不画进视频帧，单独把形状和位置发给 client，由 client 本地画，
// 这样鼠标移动时没有网络延迟的感觉。这是远程桌面"跟手"的关键之一。
//
// 实现：
//   linux    x11_cursor_monitor（XFixes）
//   windows  win_cursor_monitor（GetCursorInfo + GetIconInfo，或 DXGI 帧里的指针信息）
//   macos    mac_cursor_monitor（NSCursor.currentSystemCursor）
//
// 通用要点：
//   - 位置一般没有变化通知，需要定时查询
//   - 同一个光标反复出现时（箭头 <-> 输入框），可以缓存，只发编号（以后优化）
//
// 参考：tigervnc/common/rfb/Cursor.cxx   光标数据处理

#pragma once

import tether.protocol;

namespace tether::agent {

class CursorMonitor {
public:
    virtual ~CursorMonitor() = default;

    // 光标形状变了返回 true，并填好 out
    virtual bool poll_shape(proto::CursorShape& out) = 0;

    // 光标位置变了返回 true（远端程序自己移动了鼠标时需要通知 client）
    virtual bool poll_position(proto::CursorPos& out) = 0;
};

}  // namespace tether::agent
