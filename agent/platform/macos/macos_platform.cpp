// macOS 平台
//
// 各部件：
//   截屏      sck_capture.cpp + sck_stream.mm         ScreenCaptureKit（macOS 12.3+）
//   注入      cgevent_injector.cpp                    CGEventPost
//   光标      mac_cursor_monitor.cpp + ns_cursor.mm   NSCursor.currentSystemCursor
//   剪贴板    mac_clipboard.cpp + pasteboard.mm       NSPasteboard（轮询 changeCount）
//   显示器    mac_display.cpp                         Quartz Display Services
//   声音      sck_audio_capture.cpp + sck_stream.mm   ScreenCaptureKit 音频（macOS 13+）
//   编码      通用的 ffmpeg_encoder 就能用（h264_videotoolbox 硬件编码）
//
// .cpp 和 .mm 的分工见 objc_bridge.h。坐标（点和像素）见 backends.h。
//
// 权限问题（重要，决定 agent 能不能用）：
//   截屏需要"屏幕录制"权限，注入需要"辅助功能"权限，都是用户在系统设置里给具体程序授权。
//   权限挂在"负责进程"上：从终端启动的程序，权限算在终端 App 头上；
//   经 sshd 启动的进程一般拿不到这些权限，截屏是黑的、注入没反应。
//   做法和 Windows 类似：agent 做成用户登录后常驻的 LaunchAgent
//   （~/Library/LaunchAgents/xxx.plist），ssh 只启动一个中转程序，
//   通过 Unix socket 把 stdin/stdout 接到常驻 agent 上。
//   另外：agent 要签名（至少 ad-hoc 签名 codesign -s -），否则每次重新编译后权限都会失效
//
// 参考：
//   Sunshine/src/platform/macos/ 整个目录
//   Sunshine/src/platform/macos/misc.mm  get_permission_statuses()（第 213 行附近）各种权限的检查方式

#include "platform/platform.h"
#include "platform/macos/backends.h"

namespace tether::agent {

namespace {

class MacosPlatform final : public Platform {
public:
    const char* name() const override { return "macos"; }

    Result<std::unique_ptr<Capturer>> create_capturer(const CaptureRegion& region) override {
        return macos_backend::create_sck_capturer(region);
    }

    // CGEvent 用全局点坐标，不需要屏幕尺寸
    Result<std::unique_ptr<Injector>> create_injector(int, int) override {
        return macos_backend::create_cgevent_injector();
    }

    Result<std::unique_ptr<CursorMonitor>> create_cursor_monitor() override {
        return macos_backend::create_mac_cursor_monitor();
    }

    Result<std::unique_ptr<Clipboard>> create_clipboard() override {
        return macos_backend::create_mac_clipboard();
    }

    Result<std::unique_ptr<DisplayManager>> create_display_manager() override {
        return macos_backend::create_mac_display();
    }

    Result<std::unique_ptr<AudioCapture>> create_audio_capture() override {
#ifdef TETHER_HAVE_AUDIO
        return macos_backend::create_sck_audio_capture();
#else
        return fail(Errc::Unsupported, "built without TETHER_WITH_AUDIO");
#endif
    }
};

}  // namespace

Result<std::unique_ptr<Platform>> create_platform(const PlatformOptions&) {
    // TODO: 启动时检查一遍权限（CGPreflightScreenCaptureAccess、CGPreflightPostEventAccess），
    //       缺哪个就打一条清楚的日志，告诉用户去系统设置的哪里打开
    return std::make_unique<MacosPlatform>();
}

}  // namespace tether::agent
