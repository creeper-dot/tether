// 光标监视
//
// 光标不画进视频帧，单独把形状和位置发给 client，由 client 本地画，
// 这样鼠标移动时没有网络延迟的感觉。这是远程桌面"跟手"的关键之一。
//
// 参考：
//   tigervnc/unix/x0vncserver/XDesktop.cxx
//     第 187 行附近：XFixesSelectCursorInput 订阅光标形状变化
//     第 892 行附近：处理 XFixesCursorNotify 事件
//     第 1028 行附近：XFixesGetCursorImage 取光标图像
//   tigervnc/common/rfb/Cursor.cxx           光标数据处理
//   FreeRDP/server/shadow/X11/x11_shadow.c   搜 XFixesGetCursorImage
//
// 要点：
//   - XFixesGetCursorImage 返回的像素是 unsigned long 数组（64 位系统上每个 8 字节），
//     不是 uint32_t，转换时要逐个取低 32 位，这是个常见的坑
//   - 位置：形状变化有事件通知，位置没有，需要定时 XQueryPointer
//   - 同一个光标反复出现时（箭头 <-> 输入框），可以缓存，只发编号（以后优化）
//
// 依赖：libxfixes-dev

#pragma once

import tether.protocol;

namespace tether::agent {

class CursorMonitor {
public:
    explicit CursorMonitor(const char* display);
    ~CursorMonitor();

    // 光标形状变了返回 true，并填好 out
    bool poll_shape(proto::CursorShape& out);

    // 光标位置变了返回 true（远端程序自己移动了鼠标时需要通知 client）
    bool poll_position(proto::CursorPos& out);

private:
    // TODO: Display*、xfixes event base、上次位置
};

}  // namespace tether::agent
