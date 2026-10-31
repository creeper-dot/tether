// Wayland 下通过 portal 注入输入
//
// 参考：
//   Sunshine/src/platform/linux/portalgrab.cpp（输入相关的 Notify* 调用）
//   portal 文档 org.freedesktop.portal.RemoteDesktop：
//     NotifyPointerMotionAbsolute、NotifyPointerButton、NotifyPointerAxis、NotifyKeyboardKeycode
//
// 要点：
//   - 按键用 evdev 键码（和 uinput 一样），用 keymap.h 转换
//   - 绝对坐标需要带上 PipeWire 流的 node_id

#include "input/injector.h"
#include "portal/portal_session.h"

namespace tether::agent {

Result<std::unique_ptr<Injector>> create_portal_injector(PortalSession&) {
    // TODO
    return fail(Errc::Unsupported, "create_portal_injector not implemented");
}

}  // namespace tether::agent
