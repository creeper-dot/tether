// 显示器布局和改分辨率
//
// 功能：
//   - 列出显示器：数量、各自位置和尺寸 -> 发 proto::DisplayLayout 给 client
//   - client 选择看哪个显示器 -> 给 Capturer 的 CaptureRegion
//   - client 窗口大小变了 -> 远端改分辨率（像 RDP 那样画面始终铺满窗口）
//   - 监听显示器插拔、分辨率变化
//
// 参考：
//   tigervnc/unix/common/randr.cxx
//     computeScreenLayout()：读取当前显示器布局
//     setScreenLayout()：按 client 要求改分辨率
//   tigervnc/unix/x0vncserver/RandrGlue.c   XRandR 调用的封装，第 131、374 行
//   tigervnc/common/rfb/ScreenSet.h         布局的数据结构
//   FreeRDP/channels/disp                   RDP 的"动态改分辨率"通道，看协议设计
//
// 要点：
//   - 真实显示器只能切到它支持的模式（XRRGetScreenResources 列出来的）
//   - Xvfb 和 Xvnc 可以设任意分辨率，需要先 XRRCreateMode 再加到 output 上
//   - 改分辨率前保存原来的设置，会话结束时恢复
//
// 依赖：libxrandr-dev

#pragma once

#include <cstdint>

import tether.protocol;

namespace tether::agent {

class DisplayManager {
public:
    explicit DisplayManager(const char* display);
    ~DisplayManager();

    proto::DisplayLayout layout();

    // 尝试把指定显示器改成 w x h
    Status resize(std::uint32_t screen_id, int w, int h);

    // 会话结束时恢复原始分辨率
    void restore();

    // 布局变了（插拔、别人改了分辨率）返回 true
    bool poll_changed();

private:
    // TODO: Display*、原始配置
};

}  // namespace tether::agent
