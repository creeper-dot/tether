// xdg-desktop-portal 会话：Wayland 下截屏和注入输入都要先过这一关
//
// Wayland 不允许普通程序随便截屏、模拟输入，必须通过 portal 向用户申请权限。
// 流程（D-Bus 调用，用 GIO 的 GDBus 实现）：
//   1. RemoteDesktop.CreateSession
//   2. RemoteDesktop.SelectDevices（键盘 + 鼠标）
//   3. ScreenCast.SelectSources（选显示器，cursor_mode = metadata）
//   4. RemoteDesktop.Start -> 桌面弹出授权对话框，用户点同意
//   5. ScreenCast.OpenPipeWireRemote -> 拿到 PipeWire fd + 视频流 node_id
//   之后：Notify* 系列方法注入输入，或者 ConnectToEIS 用 libei 注入
//
// 参考：
//   Sunshine/src/platform/linux/portalgrab.cpp
//     第 33 行附近：接口名常量
//     第 440 行附近：创建 RemoteDesktop + ScreenCast 组合会话
//     第 610 行附近：persist_mode + restore_token，下次连接不用再弹窗
//   portal 官方文档 https://flatpak.github.io/xdg-desktop-portal/docs/
//     （org.freedesktop.portal.RemoteDesktop 和 ScreenCast 两节）
//   GNOME 的 gnome-remote-desktop 项目 src/grd-session.c 等（更完整的参考）
//
// 限制：
//   - 第一次必须有人在被控机前点同意。保存 restore_token 后可以免弹窗
//   - 无人登录、锁屏时 portal 不可用（这种场景需要 KMS 截屏 + uinput，难度高很多）
//
// 依赖：libglib2.0-dev（GIO）

#pragma once

#include <cstdint>
#include <string>

import tether.util;

namespace tether::agent::linux_backend {

class PortalSession {
public:
    PortalSession();
    ~PortalSession();

    // 阻塞直到用户同意或拒绝。restore_token 为空表示第一次
    Status start(const std::string& restore_token);

    int pipewire_fd() const { return pw_fd_; }
    std::uint32_t video_node_id() const { return node_id_; }

    // 下次可以用来免授权，保存到 ~/.local/state/tether/ 之类的位置
    const std::string& restore_token() const { return restore_token_; }

    // 注入输入（给 portal_injector 用）
    void notify_pointer_motion_absolute(double x, double y);
    void notify_pointer_button(int evdev_button, bool down);
    void notify_pointer_axis(double dx, double dy);
    void notify_keyboard_keycode(int evdev_keycode, bool down);

private:
    int pw_fd_ = -1;
    std::uint32_t node_id_ = 0;
    std::string restore_token_;
    // TODO: GDBusConnection*、session handle 等
};

}  // namespace tether::agent::linux_backend
