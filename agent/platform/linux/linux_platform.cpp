// Linux 平台：按会话类型选后端
//
// 后端选择（X11 / Wayland）：
//   opt.backend 为空时看环境变量 XDG_SESSION_TYPE（"x11" / "wayland"）。
//   X11      x11_capture + xtest_injector，开了 TETHER_HAVE_UINPUT 也可以选 uinput_injector
//   Wayland  portal_session + pipewire_capture + portal_injector（需要 TETHER_HAVE_PIPEWIRE）
//            Wayland 下 PortalSession 作为 LinuxPlatform 的成员，截屏和注入共用

#include "platform/platform.h"
#include "platform/linux/backends.h"

#ifdef TETHER_HAVE_PIPEWIRE
#include "platform/linux/portal_session.h"
#endif

namespace tether::agent {

namespace {

class LinuxPlatform final : public Platform {
public:
    explicit LinuxPlatform(const PlatformOptions& opt) : opt_(opt) {}

    const char* name() const override { return "linux-x11"; }

    Result<std::unique_ptr<Capturer>> create_capturer(const CaptureRegion& region) override {
        // TODO: Wayland 时走 create_pipewire_capturer(*portal_)
        return linux_backend::create_x11_capturer(opt_.display, region);
    }

    Result<std::unique_ptr<Injector>> create_injector(int, int) override {
        // TODO: Wayland 时走 create_portal_injector(*portal_)；
        //       想用 uinput 时走 create_uinput_injector(screen_w, screen_h)
        return linux_backend::create_xtest_injector(opt_.display);
    }

    Result<std::unique_ptr<CursorMonitor>> create_cursor_monitor() override {
        return linux_backend::create_x11_cursor_monitor(opt_.display);
    }

    Result<std::unique_ptr<Clipboard>> create_clipboard() override {
        return linux_backend::create_x11_clipboard(opt_.display);
    }

    Result<std::unique_ptr<DisplayManager>> create_display_manager() override {
        return linux_backend::create_xrandr_display(opt_.display);
    }

    Result<std::unique_ptr<AudioCapture>> create_audio_capture() override {
#ifdef TETHER_HAVE_AUDIO
        return linux_backend::create_pulse_audio_capture();
#else
        return fail(Errc::Unsupported, "built without TETHER_WITH_AUDIO");
#endif
    }

private:
    PlatformOptions opt_;
#ifdef TETHER_HAVE_PIPEWIRE
    std::unique_ptr<linux_backend::PortalSession> portal_;   // 只有 Wayland 用
#endif
};

}  // namespace

Result<std::unique_ptr<Platform>> create_platform(const PlatformOptions& opt) {
    // TODO: 解析 opt.backend / XDG_SESSION_TYPE；Wayland 时先建 PortalSession 并 start()
    return std::make_unique<LinuxPlatform>(opt);
}

}  // namespace tether::agent
