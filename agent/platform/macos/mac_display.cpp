// macOS 显示器布局和改分辨率
//
// 列出显示器 layout()：
//   CGGetActiveDisplayList(max, ids, &count)
//   CGDisplayBounds(id)          位置和大小，单位是点 -> 乘缩放比例换成像素
//   CGDisplayIsMain(id)          是否主屏
//   screen id 直接用 CGDirectDisplayID（同一次开机内稳定）
//
// 缩放比例 main_display_scale()：
//   CGDisplayModeRef m = CGDisplayCopyDisplayMode(CGMainDisplayID());
//   scale = (double)CGDisplayModeGetPixelWidth(m) / CGDisplayModeGetWidth(m);
//   CGDisplayModeRelease(m);
//
// 改分辨率 resize()：
//   CGDisplayCopyAllDisplayModes(id, options) 列出所有模式
//     options 里放 kCGDisplayShowDuplicateLowResolutionModes = true 才能看到 HiDPI 模式
//   找 PixelWidth/PixelHeight 最接近的，第一次改之前 CGDisplayCopyDisplayMode 保存原始模式
//   CGBeginDisplayConfiguration -> CGConfigureDisplayWithDisplayMode -> CGCompleteDisplayConfiguration(cfg, kCGConfigureForSession)
//     kCGConfigureForSession：只对本次登录有效，注销后恢复
//
// 检测变化 poll_changed()：
//   CGDisplayRegisterReconfigurationCallback 注册回调，回调里置一个 atomic<bool> 标志
//   回调在主线程 run loop 上调用；agent 没有跑 run loop 时收不到，那就退回"每次重新枚举比较"
//
// 参考：
//   Sunshine/src/platform/macos/display.mm
//     active_desktop_bounds()（第 59 行附近）   CGGetActiveDisplayList + CGDisplayBounds
//     display_names()（第 324 行附近）
//   Apple 文档 "Quartz Display Services"

#include "platform/macos/backends.h"

// #include <CoreGraphics/CoreGraphics.h>

namespace tether::agent::macos_backend {

namespace {

class MacDisplay final : public DisplayManager {
public:
    proto::DisplayLayout layout() override {
        // TODO
        return {};
    }

    Status resize(std::uint32_t, int, int) override {
        // TODO
        return fail(Errc::Unsupported, "macos resize not implemented");
    }

    void restore() override {
        // TODO: CGRestorePermanentDisplayConfiguration() 或者逐个设回保存的模式
    }

    bool poll_changed() override {
        // TODO
        return false;
    }

private:
    // TODO: 改过的显示器 id 和原始 CGDisplayModeRef、上次的 layout
};

}  // namespace

double main_display_scale() {
    // TODO: 见文件开头
    return 1.0;
}

Result<std::unique_ptr<DisplayManager>> create_mac_display() {
    return std::make_unique<MacDisplay>();
}

}  // namespace tether::agent::macos_backend
