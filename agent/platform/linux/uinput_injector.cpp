// /dev/uinput 虚拟输入设备
//
// 原理：在内核里创建一个虚拟键盘和鼠标，系统会把它当成真插了一个 USB 设备。
// 所以 X11、Wayland、甚至文本控制台都能收到输入，不依赖图形环境。
//
// 参考：
//   内核文档 https://www.kernel.org/doc/html/latest/input/uinput.html（自带完整示例）
//   Sunshine/src/platform/linux/input/virtualhid.cpp 和 third-party/libvirtualhid
//     （Sunshine 新版用的封装，鼠标、键盘、触控板、手柄都有）
//   /usr/include/linux/input-event-codes.h   KEY_*、BTN_*、REL_*、ABS_* 定义
//
// 要点：
//   - 需要 /dev/uinput 的写权限：把用户加到 input 组，或者装一条 udev 规则
//     KERNEL=="uinput", GROUP="input", MODE="0660"
//   - 绝对坐标鼠标：开 EV_ABS 的 ABS_X/ABS_Y，范围设成屏幕尺寸，
//     并加 INPUT_PROP_DIRECT 之类属性让桌面把它当成绝对设备
//   - 滚轮：REL_WHEEL（整格）+ REL_WHEEL_HI_RES（1/120 格）
//   - 每组事件后要发一个 SYN_REPORT
//   - 创建设备后要等一小会儿（几十毫秒）桌面才认到它

#include "platform/linux/backends.h"
#include "platform/linux/keymap.h"

// #include <fcntl.h>
// #include <linux/uinput.h>

namespace tether::agent::linux_backend {

Result<std::unique_ptr<Injector>> create_uinput_injector(int, int) {
    // TODO: 定义 class UinputInjector : public Injector
    return fail(Errc::Unsupported, "create_uinput_injector not implemented");
}

}  // namespace tether::agent::linux_backend
