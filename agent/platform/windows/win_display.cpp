// Windows 显示器布局和改分辨率
//
// 列出显示器 layout()：
//   EnumDisplayMonitors(nullptr, nullptr, callback, ...)，callback 里 GetMonitorInfoW(hmon, &MONITORINFOEXW)
//     rcMonitor            在虚拟桌面里的位置和大小
//     dwFlags & MONITORINFOF_PRIMARY   是否主屏
//     szDevice             设备名，比如 \\.\DISPLAY1，改分辨率时要用
//   desktop_width/height 用 GetSystemMetrics(SM_CXVIRTUALSCREEN / SM_CYVIRTUALSCREEN)
//   screen id：用 EnumDisplayMonitors 的顺序就行，但要保证同一会话里稳定（插拔后可能变）
//
// 改分辨率 resize()：
//   1. EnumDisplaySettingsW(szDevice, i, &devmode) 从 i=0 开始枚举，找 dmPelsWidth/Height 相同
//      （或最接近）的模式；真实显示器不能设任意分辨率
//   2. 第一次改之前 EnumDisplaySettingsW(szDevice, ENUM_CURRENT_SETTINGS, ...) 保存原始模式
//   3. ChangeDisplaySettingsExW(szDevice, &devmode, nullptr, CDS_FULLSCREEN, nullptr)
//      CDS_FULLSCREEN 表示临时修改，不写注册表；进程退出系统会自动恢复（但最好还是 restore()）
//   想要任意分辨率需要虚拟显示器驱动（IddCx，比如 Virtual Display Driver 项目），以后再说
//
// 恢复 restore()：对每个改过的显示器 ChangeDisplaySettingsExW(szDevice, nullptr, nullptr, 0, nullptr)
//
// 检测变化 poll_changed()：
//   简单做法：每次重新枚举一遍和上次比较（显示器数量很少，开销可以忽略）
//   正规做法：隐藏窗口收 WM_DISPLAYCHANGE
//   另外 DXGI 截屏遇到 ACCESS_LOST 时一般也意味着布局变了
//
// 参考：
//   Sunshine/src/platform/windows/display_base.cpp   搜 DesktopCoordinates、EnumOutputs（用 DXGI 枚举）
//   Sunshine 的分辨率切换在单独的 libdisplaydevice 项目里（LizardByte/libdisplaydevice），
//     用的是更新的 SetDisplayConfig / QueryDisplayConfig API，功能更全也更复杂
//   tigervnc/win/rfb_win32/MonitorInfo.cxx、DisplayDevice 相关
//   learn.microsoft.com "ChangeDisplaySettingsExW"

#include "platform/windows/backends.h"
#include "platform/windows/win_util.h"

namespace tether::agent::windows_backend {

namespace {

class WinDisplay final : public DisplayManager {
public:
    proto::DisplayLayout layout() override {
        // TODO: EnumDisplayMonitors + GetMonitorInfoW
        return {};
    }

    Status resize(std::uint32_t, int, int) override {
        // TODO: EnumDisplaySettingsW 找模式 -> 保存原始 -> ChangeDisplaySettingsExW(CDS_FULLSCREEN)
        return fail(Errc::Unsupported, "windows resize not implemented");
    }

    void restore() override {
        // TODO
    }

    bool poll_changed() override {
        // TODO: 重新枚举，和上次的 layout 比较
        return false;
    }

private:
    // TODO: 每个显示器的设备名（std::wstring）、改过的显示器的原始 DEVMODEW、上次的 layout
};

}  // namespace

Result<std::unique_ptr<DisplayManager>> create_win_display() {
    return std::make_unique<WinDisplay>();
}

}  // namespace tether::agent::windows_backend
