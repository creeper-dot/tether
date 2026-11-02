// Windows 输入注入：SendInput
//
// 鼠标：
//   绝对移动  MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_VIRTUALDESK
//             坐标不是像素，要换算到 0~65535：
//               vx = GetSystemMetrics(SM_XVIRTUALSCREEN)，vw = SM_CXVIRTUALSCREEN（虚拟桌面原点和宽度）
//               dx = (x - vx) * 65535 / (vw - 1)
//             不加 VIRTUALDESK 时 0~65535 只对应主显示器
//   相对移动  MOUSEEVENTF_MOVE，dx/dy 是像素，但会受"提高指针精确度"（鼠标加速）影响
//   按键      1 左 LEFTDOWN/LEFTUP，2 中 MIDDLE*，3 右 RIGHT*，
//             8/9 后退/前进 XDOWN/XUP + mouseData = XBUTTON1 / XBUTTON2
//   滚轮      MOUSEEVENTF_WHEEL（竖）、MOUSEEVENTF_HWHEEL（横），mouseData 就是 1/120 格
//             （WHEEL_DELTA = 120，和协议单位一致，不用换算）
//             注意方向：协议 dy > 0 是往下滚，Windows WHEEL 正数是往上，要取反
//
// 键盘：见 keymap.h
//   扫描码    KEYEVENTF_SCANCODE，wScan = 扫描码，扩展键加 KEYEVENTF_EXTENDEDKEY，抬起加 KEYEVENTF_KEYUP
//   VK        wVk = VK 码
//   Unicode   KEYEVENTF_UNICODE，wScan = UTF-16 码元（BMP 以外的字符要拆成两个代理项各发一次）
//
// 失败处理：
//   SendInput 返回值是成功插入的事件数，0 表示被拦了。常见原因：
//     - 当前线程不在输入桌面上（锁屏、UAC）-> sync_input_desktop() 后重试一次
//     - UIPI：被控的窗口是管理员权限运行的，而 agent 不是 -> 注入不进去，只能提权运行 agent
//
// release_all()：
//   记录按下的扫描码 / VK / 鼠标键（std::set 之类），逐个发抬起事件
//
// 参考：
//   Sunshine v0.23.1 src/platform/windows/input.cpp
//     （新版改用 libvirtualhid，SendInput 部分在旧版本里更清楚）
//     send_input()（第 472 行附近）     SendInput 失败后 syncThreadDesktop 重试
//     abs_mouse()（第 509 行附近）      0~65535 换算、MOUSEEVENTF_VIRTUALDESK
//     button_mouse()（第 546 行附近）   XBUTTON1/2
//     scroll() / hscroll()（第 574 行附近）
//     keyboard()（第 600 行附近）       扫描码、扩展键
//     unicode()（第 1157 行附近）       KEYEVENTF_UNICODE、UTF-16 代理项
//   learn.microsoft.com "SendInput"、"MOUSEINPUT"、"KEYBDINPUT"
//
// 依赖：user32（系统自带）

#include "platform/windows/backends.h"
#include "platform/windows/keymap.h"
#include "platform/windows/win_util.h"

namespace tether::agent::windows_backend {

namespace {

class SendInputInjector final : public Injector {
public:
    void mouse_move(int, int) override {
        // TODO: 换算到 0~65535，MOUSEEVENTF_MOVE | ABSOLUTE | VIRTUALDESK
    }

    void mouse_move_rel(int, int) override {
        // TODO: MOUSEEVENTF_MOVE
    }

    void mouse_button(int, bool) override {
        // TODO: 按 button 选 LEFT/MIDDLE/RIGHT/X，记录按下状态
    }

    void mouse_scroll(int, int) override {
        // TODO: dy -> MOUSEEVENTF_WHEEL（取反），dx -> MOUSEEVENTF_HWHEEL
    }

    void key(std::uint32_t, std::uint32_t, bool) override {
        // TODO: 有 scancode 用扫描码；否则 keysym_to_vk；还不行且是 Unicode 字符就发 KEYEVENTF_UNICODE
        //       按下时记录，抬起时删除
    }

    void release_all() override {
        // TODO
    }

private:
    // TODO: 这里统一调 ::SendInput，返回 0 时 sync_input_desktop() 再试一次
    // void send(INPUT* inputs, UINT count);

    // TODO: 按下的扫描码、VK、鼠标键
};

}  // namespace

Result<std::unique_ptr<Injector>> create_sendinput_injector() {
    // TODO: 先 sync_input_desktop()，失败只打警告（agent 本来就在 Default 桌面时不影响）
    return std::make_unique<SendInputInjector>();
}

}  // namespace tether::agent::windows_backend
