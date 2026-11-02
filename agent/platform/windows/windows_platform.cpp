// Windows 平台
//
// 各部件：
//   截屏      dxgi_capture.cpp          DXGI Desktop Duplication
//   注入      sendinput_injector.cpp    SendInput
//   光标      win_cursor_monitor.cpp    GetCursorInfo + GetIconInfo
//   剪贴板    win_clipboard.cpp         CF_UNICODETEXT + GetClipboardSequenceNumber
//   显示器    win_display.cpp           EnumDisplayMonitors / ChangeDisplaySettingsExW
//   声音      wasapi_audio_capture.cpp  WASAPI loopback
//   编码      通用的 ffmpeg_encoder 就能用（h264_nvenc / h264_amf / h264_qsv / libx264）
//   工具      win_util.h                UTF-8/UTF-16 转换、切换输入桌面、HRESULT 转 Error
//
// 进程级初始化（create_platform 里做一次）：
//   SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2)
//     不设的话，在缩放 125%/150% 的屏幕上，GetCursorInfo、EnumDisplayMonitors 给的都是缩放后的
//     假坐标，和 DXGI 截到的真实像素对不上，鼠标会点歪。也可以写在程序的 manifest 里
//
// 会话问题（重要，决定 agent 能不能用）：
//   OpenSSH Server 启动的进程不在用户的桌面会话里（运行在服务会话），截不到屏、也注入不了输入。
//   可行的做法：
//     A. agent 常驻在用户会话里（登录后自启或计划任务），ssh 只启动一个中转程序，
//        通过命名管道（\\.\pipe\tether-agent）把 stdin/stdout 接到常驻 agent 上
//     B. 装一个以 SYSTEM 运行的服务，用 WTSGetActiveConsoleSessionId + WTSQueryUserToken
//        + CreateProcessAsUserW 把 agent 启动到活动会话里（Sunshine 就是这样做的，
//        SYSTEM 权限还能截到锁屏和 UAC 安全桌面）
//   建议先用 A：先在被控机上手动运行 agent 调通，再考虑 B
//   参考 Sunshine/src/platform/windows/misc.cpp：
//     retrieve_users_token()（第 318 行附近）  WTSGetActiveConsoleSessionId + WTSQueryUserToken
//     run_command()（第 1006 行附近）          CreateProcessAsUserW
//     create_startup_info()（第 622 行附近）   STARTUPINFOEXW + PROC_THREAD_ATTRIBUTE_HANDLE_LIST
//
// 参考：Sunshine/src/platform/windows/ 整个目录

#include "platform/platform.h"
#include "platform/windows/backends.h"
#include "platform/windows/win_util.h"

namespace tether::agent {

namespace {

class WindowsPlatform final : public Platform {
public:
    const char* name() const override { return "windows"; }

    Result<std::unique_ptr<Capturer>> create_capturer(const CaptureRegion& region) override {
        return windows_backend::create_dxgi_capturer(region);
    }

    // SendInput 用虚拟桌面坐标，不需要屏幕尺寸
    Result<std::unique_ptr<Injector>> create_injector(int, int) override {
        return windows_backend::create_sendinput_injector();
    }

    Result<std::unique_ptr<CursorMonitor>> create_cursor_monitor() override {
        return windows_backend::create_win_cursor_monitor();
    }

    Result<std::unique_ptr<Clipboard>> create_clipboard() override {
        return windows_backend::create_win_clipboard();
    }

    Result<std::unique_ptr<DisplayManager>> create_display_manager() override {
        return windows_backend::create_win_display();
    }

    Result<std::unique_ptr<AudioCapture>> create_audio_capture() override {
#ifdef TETHER_HAVE_AUDIO
        return windows_backend::create_wasapi_audio_capture();
#else
        return fail(Errc::Unsupported, "built without TETHER_WITH_AUDIO");
#endif
    }
};

}  // namespace

Result<std::unique_ptr<Platform>> create_platform(const PlatformOptions&) {
    // TODO: SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2)，
    //       失败（已经在 manifest 里设过时会失败）只打日志
    // TODO: 检查自己是不是在交互会话里（ProcessIdToSessionId + WTSGetActiveConsoleSessionId 比较），
    //       不是的话直接报错，提示用户看上面的"会话问题"
    return std::make_unique<WindowsPlatform>();
}

}  // namespace tether::agent
