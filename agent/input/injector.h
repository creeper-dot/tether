// 输入注入接口
//
// 实现：
//   xtest_injector   X11 XTest 扩展（MVP）
//   uinput_injector  内核虚拟输入设备，X11/Wayland/控制台都能用，需要 /dev/uinput 权限
//   portal_injector  Wayland 下通过 xdg-desktop-portal 注入，不需要特殊权限
//
// 参考：
//   Sunshine/src/input.cpp                  协议输入 -> 平台调用的分发，按键状态跟踪
//   FreeRDP/server/shadow/shadow_input.c    RDP 服务端的输入分发
//   tigervnc/unix/x0vncserver/XDesktop.cxx  X11 实现

#pragma once

#include <cstdint>
#include <memory>

import tether.util;

namespace tether::agent {

class Injector {
public:
    virtual ~Injector() = default;

    virtual void mouse_move(int x, int y) = 0;              // 绝对坐标
    virtual void mouse_move_rel(int dx, int dy) = 0;        // 相对移动
    virtual void mouse_button(int button, bool down) = 0;   // 1 左 2 中 3 右 8 后退 9 前进
    virtual void mouse_scroll(int dx_120, int dy_120) = 0;  // 1/120 格
    virtual void key(std::uint32_t keysym, std::uint32_t scancode, bool down) = 0;

    // 松开所有按下的键和鼠标键（client 失去焦点、断线时调用，防止远端按键卡住）
    // 实现时需要记录哪些键当前是按下的
    virtual void release_all() = 0;
};

Result<std::unique_ptr<Injector>> create_xtest_injector(const char* display);

#ifdef TETHER_HAVE_UINPUT
// width/height 是屏幕尺寸，绝对坐标设备需要知道范围
Result<std::unique_ptr<Injector>> create_uinput_injector(int width, int height);
#endif

#ifdef TETHER_HAVE_PIPEWIRE
class PortalSession;
Result<std::unique_ptr<Injector>> create_portal_injector(PortalSession& portal);
#endif

}  // namespace tether::agent
