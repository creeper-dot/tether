// macOS 输入注入：CGEvent
//
// 权限：需要"辅助功能"权限（系统设置 -> 隐私与安全性 -> 辅助功能），
//   没有权限时 CGEventPost 不报错，事件直接被丢掉。
//   AXIsProcessTrusted() 或 CGPreflightPostEventAccess()（macOS 10.15+）可以检查，
//   CGRequestPostEventAccess() 会弹出授权提示
//
// 事件源：
//   CGEventSourceRef src = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);
//   所有事件都用它创建，模拟的是"硬件"输入。用完 CFRelease
//
// 鼠标：坐标单位是点，不是像素（见 backends.h），先除以 main_display_scale()
//   绝对移动  CGEventCreateMouseEvent(src, type, point, button)
//             type 要看当前有没有按键按着：没按是 kCGEventMouseMoved，
//             按着左键是 kCGEventLeftMouseDragged，右键 kCGEventRightMouseDragged，
//             其他键 kCGEventOtherMouseDragged。用错了拖动（选文字、拖窗口）不生效
//   相对移动  macOS 没有直接的相对移动事件：取当前位置（CGEventGetLocation(CGEventCreate(nullptr))）
//             加上偏移，再发绝对移动；同时 CGEventSetIntegerValueField(ev, kCGMouseEventDeltaX, dx)
//             让游戏能读到原始位移
//   按键      kCGEventLeftMouseDown/Up、kCGEventRightMouseDown/Up、kCGEventOtherMouseDown/Up
//             （中键、后退、前进用 Other，button 分别是 kCGMouseButtonCenter、3、4）
//   双击      macOS 不会自己判断双击，要自己记录：短时间（NSEvent.doubleClickInterval）内
//             同一位置再次按下，用 CGEventSetIntegerValueField(ev, kCGMouseEventClickState, 2)
//             不做的话双击打开文件、双击选词都不灵
//   滚轮      CGEventCreateScrollWheelEvent2(src, kCGScrollEventUnitPixel, 2, dy, dx, 0)
//             协议单位 1/120 格；换成像素大约 dy_120 / 120 * 10（一格 10 像素左右，可以调）
//             方向：协议 dy > 0 往下滚，macOS 正数往上，要取反（"自然滚动"开关会再反一次，不用管）
//
// 键盘：见 keymap.h
//   CGEventCreateKeyboardEvent(src, keycode, down)
//   修饰键要自己维护 flags：按下 Shift 后，之后每个键盘事件都要 CGEventSetFlags(ev, flags)，
//   否则 Shift+A 出来的还是小写 a
//   没有 keycode 的 Unicode 字符：CGEventKeyboardSetUnicodeString(ev, len, utf16)
//
// 发送：CGEventPost(kCGHIDEventTap, ev); CFRelease(ev);
//
// 参考：
//   Sunshine v0.23.1 src/platform/macos/input.cpp
//     （新版改用 libvirtualhid，CGEvent 的写法在旧版本里）
//     keyboard()（第 239 行附近）          CGEventSetFlags 维护修饰键
//     post_mouse()（第 324 行附近）        kCGMouseEventClickState 双击判断
//     event_type_mouse()（第 354 行附近）  按下状态决定 Moved 还是 Dragged
//     move_mouse() / abs_mouse()（第 372、381 行附近）
//     scroll()（第 429 行附近）            CGEventCreateScrollWheelEvent
//     input()（第 507 行附近）             CGEventSourceCreate(kCGEventSourceStateHIDSystemState)
//   Sunshine/src/platform/macos/misc.mm  request_input_post_event()（第 184 行附近）检查和申请权限
//   Apple 文档 "Quartz Event Services"
//
// 依赖：ApplicationServices（CoreGraphics）框架

#include "platform/macos/backends.h"
#include "platform/macos/keymap.h"

// #include <ApplicationServices/ApplicationServices.h>

namespace tether::agent::macos_backend {

namespace {

class CgEventInjector final : public Injector {
public:
    ~CgEventInjector() override {
        // TODO: CFRelease(source_)
    }

    Status init() {
        // TODO: CGPreflightPostEventAccess() 为 false 时 CGRequestPostEventAccess()，
        //       仍然没有权限就 return fail(Errc::System, "need Accessibility permission")
        //       创建 CGEventSource
        return fail(Errc::Unsupported, "cgevent injector not implemented");
    }

    void mouse_move(int, int) override {
        // TODO: 像素 -> 点，按当前按下的键选 Moved / Dragged
    }

    void mouse_move_rel(int, int) override {
        // TODO: 当前位置 + 偏移，设 kCGMouseEventDeltaX/Y
    }

    void mouse_button(int, bool) override {
        // TODO: 选 Left/Right/Other，处理双击计数，记录按下状态
    }

    void mouse_scroll(int, int) override {
        // TODO: CGEventCreateScrollWheelEvent2，方向取反
    }

    void key(std::uint32_t, std::uint32_t, bool) override {
        // TODO: evdev_to_cgkeycode(scancode)，找不到再 keysym_to_cgkeycode(keysym)，
        //       是修饰键就更新 flags_，CGEventSetFlags 后发送
    }

    void release_all() override {
        // TODO: 抬起所有按下的键和鼠标键，flags_ 清零
    }

private:
    std::uint64_t flags_ = 0;          // 当前按下的修饰键（CGEventFlags）
    // TODO: CGEventSourceRef source_（用 void* 存，或者 include ApplicationServices 后直接用）
    //       按下的 keycode、鼠标键、上次点击的时间和位置（双击用）
};

}  // namespace

Result<std::unique_ptr<Injector>> create_cgevent_injector() {
    auto inj = std::make_unique<CgEventInjector>();
    if (auto st = inj->init(); !st) {
        return std::unexpected(st.error());
    }
    return inj;
}

}  // namespace tether::agent::macos_backend
