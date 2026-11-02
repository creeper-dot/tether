// 显示器布局和改分辨率接口（各平台通用）
//
// 功能：
//   - 列出显示器：数量、各自位置和尺寸 -> 发 proto::DisplayLayout 给 client
//   - client 选择看哪个显示器 -> 给 Capturer 的 CaptureRegion
//   - client 窗口大小变了 -> 远端改分辨率（像 RDP 那样画面始终铺满窗口）
//   - 监听显示器插拔、分辨率变化
//
// 实现：
//   linux    xrandr_display（XRandR）
//   windows  win_display（EnumDisplayMonitors、ChangeDisplaySettingsEx）
//   macos    mac_display（CGGetActiveDisplayList、CGDisplaySetDisplayMode）
//
// 通用要点：
//   - 真实显示器只能切到它支持的模式
//   - 改分辨率前保存原来的设置，会话结束时恢复
//
// 参考：
//   tigervnc/common/rfb/ScreenSet.h         布局的数据结构
//   FreeRDP/channels/disp                   RDP 的"动态改分辨率"通道，看协议设计

#pragma once

#include <cstdint>

import tether.protocol;

namespace tether::agent {

class DisplayManager {
public:
    virtual ~DisplayManager() = default;

    virtual proto::DisplayLayout layout() = 0;

    // 尝试把指定显示器改成 w x h
    virtual Status resize(std::uint32_t screen_id, int w, int h) = 0;

    // 会话结束时恢复原始分辨率
    virtual void restore() = 0;

    // 布局变了（插拔、别人改了分辨率）返回 true
    virtual bool poll_changed() = 0;
};

}  // namespace tether::agent
